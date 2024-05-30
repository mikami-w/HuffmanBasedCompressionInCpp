#include"HFMTree.h"

HFMTree::HFMTree(int nodecount, unsigned int* pweight) :m_length(nodecount)
{
	int iMaxNodeCount = 2 * m_length - 1; //Huffman树的节点总数是2n-1（n代表Huffman树的叶子节点数量）
	m_data = new HFMTreeNode[iMaxNodeCount];	//节点总数固定，所以使用数组作为内存池存储

	for (int i = 0; i < iMaxNodeCount; ++i)
	{
		//所有节点索引数据初始化为-1，表示未被使用
		m_data[i].parent = -1;
		m_data[i].lchild = -1;
		m_data[i].rchild = -1;
	}
	for (int i = 0; i < m_length; ++i)
	{
		//内存池开头存放初始节点
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
	int idx1 = 0;	//最小根权索引
	int idx2 = 0;	//最小根权索引，根权大于idx1

	int iMaxNodeCount = 2 * m_length - 1; //节点总数
	int initlength = m_length;	//初始节点数，作索引为所有初始节点的下一个索引
	for (int i = initlength; i < iMaxNodeCount; ++i)
	{
		SelectTwoMinValue(idx1, idx2);
		m_data[i].weight = m_data[idx1].weight + m_data[idx2].weight; //新节点的权值等于左右孩子之和
		m_data[i].lchild = idx1;
		m_data[i].rchild = idx2;

		m_data[idx1].parent = i;
		m_data[idx2].parent = i;

		m_length++; //当前节点数
	}
}

void HFMTree::SelectTwoMinValue(int& rtnIdx1, int& rtnIdx2)
{
	int minval1 = INT_MAX;
	int minval2 = INT_MAX;

	//找最小值
	for (int i = 0; i < m_length; ++i)
	{
		if (m_data[i].parent == -1) //父标记未被使用 即当前节点为树根
		{
			if (minval1 > m_data[i].weight)
			{
				minval1 = m_data[i].weight; //记录最小值
				rtnIdx1 = i; //记录下标
			}
		}
	}

	//找第二个最小的值
	for (int i = 0; i < m_length; ++i)
	{
		if (m_data[i].parent == -1 && i != rtnIdx1) //把第一个找到的最小权值的节点排除
		{
			if (minval2 > m_data[i].weight)
			{
				minval2 = m_data[i].weight; //记录最小值
				rtnIdx2 = i; //记录下标
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
	int leafNodeCount = (m_length + 1) / 2;	//叶子数量

	if (idx < 0 || idx >= leafNodeCount)
	{
		//只允许对叶子节点求Huffman编码
		return false;
	}
	result = ""; //保存最终生成的Huffman编码
	int curridx = idx;
	int tmpparent = m_data[curridx].parent;
	while (tmpparent != -1) //沿着叶子向上回溯
	{
		if (m_data[tmpparent].lchild == curridx)
		{
			//若当前节点为左孩子则前插0
			result = "0" + result;
		}
		else
		{
			//否则当前节点为右孩子 前插1
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
	}//向上寻找root

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

