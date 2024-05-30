#include"HFMTree.h"

HFMTree::HFMTree(int nodecount, unsigned int* pweight) :m_length(nodecount)
{
	int iMaxNodeCount = 2 * m_length - 1; //Huffman���Ľڵ�������2n-1��n����Huffman����Ҷ�ӽڵ�������
	m_data = new HFMTreeNode[iMaxNodeCount];	//�ڵ������̶�������ʹ��������Ϊ�ڴ�ش洢

	for (int i = 0; i < iMaxNodeCount; ++i)
	{
		//���нڵ��������ݳ�ʼ��Ϊ-1����ʾδ��ʹ��
		m_data[i].parent = -1;
		m_data[i].lchild = -1;
		m_data[i].rchild = -1;
	}
	for (int i = 0; i < m_length; ++i)
	{
		//�ڴ�ؿ�ͷ��ų�ʼ�ڵ�
		m_data[i].weight = pweight[i];
	}

	_create();
}

HFMTree::HFMTree(const HFMTree& tree)
{
	int iMaxNodeCount = 2 * m_length - 1;
	m_data = new HFMTreeNode[iMaxNodeCount];
	memcpy(m_data, tree.m_data, iMaxNodeCount * sizeof(HFMTreeNode));
}

HFMTree::~HFMTree()
{
	delete[] m_data;
}

void HFMTree::_create()
{
	int idx1 = 0;	//��С��Ȩ����
	int idx2 = 0;	//��С��Ȩ��������Ȩ����idx1

	int iMaxNodeCount = 2 * m_length - 1; //�ڵ�����
	int initlength = m_length;	//��ʼ�ڵ�����������Ϊ���г�ʼ�ڵ����һ������
	for (int i = initlength; i < iMaxNodeCount; ++i)
	{
		SelectTwoMinValue(idx1, idx2);
		m_data[i].weight = m_data[idx1].weight + m_data[idx2].weight; //�½ڵ��Ȩֵ�������Һ���֮��
		m_data[i].lchild = idx1;
		m_data[i].rchild = idx2;

		m_data[idx1].parent = i;
		m_data[idx2].parent = i;

		m_length++; //��ǰ�ڵ���
	}
}

void HFMTree::SelectTwoMinValue(int& rtnIdx1, int& rtnIdx2)
{
	int minval1 = INT_MAX;
	int minval2 = INT_MAX;

	//����Сֵ
	for (int i = 0; i < m_length; ++i)
	{
		if (m_data[i].parent == -1) //�����δ��ʹ�� ����ǰ�ڵ�Ϊ����
		{
			if (minval1 > m_data[i].weight)
			{
				minval1 = m_data[i].weight; //��¼��Сֵ
				rtnIdx1 = i; //��¼�±�
			}
		}
	}

	//�ҵڶ�����С��ֵ
	for (int i = 0; i < m_length; ++i)
	{
		if (m_data[i].parent == -1 && i != rtnIdx1) //�ѵ�һ���ҵ�����СȨֵ�Ľڵ��ų�
		{
			if (minval2 > m_data[i].weight)
			{
				minval2 = m_data[i].weight; //��¼��Сֵ
				rtnIdx2 = i; //��¼�±�
			}
		}
	}
}

int HFMTree::GetLength()
{
	return m_length;
}

bool HFMTree::CreateHFMCode(std::string& result, int idx)
{
	int leafNodeCount = (m_length + 1) / 2;	//Ҷ������

	if (idx < 0 || idx >= leafNodeCount)
	{
		//ֻ�����Ҷ�ӽڵ���Huffman����
		return false;
	}
	result = ""; //�����������ɵ�Huffman����
	int curridx = idx;
	int tmpparent = m_data[curridx].parent;
	while (tmpparent != -1) //����Ҷ�����ϻ���
	{
		if (m_data[tmpparent].lchild == curridx)
		{
			//����ǰ�ڵ�Ϊ������ǰ��0
			result = "0" + result;
		}
		else
		{
			//����ǰ�ڵ�Ϊ�Һ��� ǰ��1
			result = "1" + result;
		}
		curridx = tmpparent;
		tmpparent = m_data[curridx].parent;
	} 
	return true;
}

HFMTreeNode* const HFMTree::getData() const
{
	return m_data;
}

int HFMTree::getRoot() const
{
	int root = 0;
	while (m_data[root].parent != -1) {
		root = m_data[root].parent;
	}//����Ѱ��root

	return root;
}

int HFMTree::getLeftChild(int idx)
{
	return m_data[idx].lchild;
}

int HFMTree::getRightChild(int idx)
{
	return m_data[idx].rchild;
}

void HFMTree::preOrder() const
{
	_preOrder(getRoot());
}

const HFMTreeNode& HFMTree::operator[](int idx) const
{
	return m_data[idx];
}

void HFMTree::_preOrder(int idx) const
{
	if (idx == -1)
		return;
	std::cout << m_data[idx].weight << ' ';
	_preOrder(m_data[idx].lchild);
	_preOrder(m_data[idx].rchild);
}

