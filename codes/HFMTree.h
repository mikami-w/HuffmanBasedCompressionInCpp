#pragma once
#include<climits>
#include<string>
#include<iostream>

typedef struct HFMTreeNode {
	int weight;	//权值
	int parent; //父亲（数组下标值）
	int lchild; //左孩子
	int rchild; //右孩子
}node;

class HFMTree {

	HFMTreeNode* m_data;	//指向内存池首地址
	int m_length;	//使用节点个数
	void _create();	//创建Huffman树
	void SelectTwoMinValue(int& rtnIdx1, int& rtnIdx2);	//找到根权重最小的两个节点索引
	void _preOrder(int idx) const;

public:
	HFMTree(int nodecount, unsigned int* pweight);
	HFMTree(const HFMTree& tree);
	~HFMTree();
	int GetLength();	//获取树中节点数量
	bool CreateHFMCode(std::string& result, int idx);	//生成当前内存池中idx位置节点的HFM编码,保存在result中
	HFMTreeNode* const getData() const;	//返回内存池首地址常量m_data
	int getRoot() const;	//返回root在内存池中索引
	int getLeftChild(int idx);	//返回idx的左孩子索引
	int getRightChild(int idx);
	void preOrder() const;	//前序遍历
	const HFMTreeNode& operator[](int idx) const;

};