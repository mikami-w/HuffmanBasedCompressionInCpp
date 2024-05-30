#pragma once
#include<climits>
#include<string>
#include<iostream>

typedef struct HFMTreeNode {
	int weight;	//Ȩֵ
	int parent; //���ף������±�ֵ��
	int lchild; //����
	int rchild; //�Һ���
}node;

class HFMTree {

	HFMTreeNode* m_data;	//ָ���ڴ���׵�ַ
	int m_length;	//ʹ�ýڵ����
	void _create();	//����Huffman��
	void SelectTwoMinValue(int& rtnIdx1, int& rtnIdx2);	//�ҵ���Ȩ����С�������ڵ�����
	void _preOrder(int idx) const;

public:
	HFMTree(int nodecount, unsigned int* pweight);
	HFMTree(const HFMTree& tree);
	~HFMTree();
	int GetLength();	//��ȡ���нڵ�����
	bool CreateHFMCode(std::string& result, int idx);	//���ɵ�ǰ�ڴ����idxλ�ýڵ��HFM����,������result��
	HFMTreeNode* const getData() const;	//�����ڴ���׵�ַ����m_data
	int getRoot() const;	//����root���ڴ��������
	int getLeftChild(int idx);	//����idx����������
	int getRightChild(int idx);
	void preOrder() const;	//ǰ�����
	const HFMTreeNode& operator[](int idx) const;

};