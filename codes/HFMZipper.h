#pragma once
#include"HFMTree.h"
#include<string>
#include<fstream>
#include<unordered_map>
#define nLeaf 256

typedef bool HMFopreator;
const int _1MB = 1024 * 1024;
const int _dict = 4 * _1MB;	//�ֵ��С4MB

namespace hfm{
	const HMFopreator compress = 1;
	const HMFopreator uncompress = 0;
}


struct fileHead {
	char fileIdentity[4] = "hFM";	//�ļ�ͷ��־
	unsigned int byteFrequency[256]{ 0 };	//���Ƶ��
};


class HFMZipper {

	std::string path;
	size_t file_sz;
	fileHead head{ 0 };
	HFMTree* hfmtree;
	std::unordered_map<unsigned char, std::string> map;//�ֽ��뵽hfm�����ӳ��
	std::unordered_map<std::string, unsigned char> map_re;//hfm���뵽�ֽ����ӳ��

	size_t getSize();
	size_t getSizeFromTree();	//���ĸ��ڵ㼴��ԭʼ�ļ���С
	void calculateFrequency();	//��ȡ�ļ�������ֽ���Ƶ��
	void readFrequency();		//ֱ�Ӷ�ȡ�ļ�ͷ�õ�Ƶ��
	void createHuffmanTree();	//�����ļ�ͷ��byteFrequency����Huffman��
	void getMap();				//����Huffman�������ֽ��뵽hfm�����ӳ��map
	void getMap_reverse();		//����Huffman������hfm���뵽�ֽ����ӳ��map_re

public:
	HFMZipper(std::string& path, HMFopreator op = hfm::compress);
	~HFMZipper();
	bool doCompress();
	bool doUncompress();
	HFMTree* const getHFMTree() const;	//���س�Աhuffman
	void showMap() const;				//���map
	void showMap_reverse() const;		//���map_re
	void showHead() const;				//����ļ�ͷ�е�Ƶ��

};