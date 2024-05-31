#pragma once
#include"HFMTree.h"
#include<string>
#include<fstream>
#include<unordered_map>
#define nLeaf 256

typedef bool HMFopreator;
const int _1MB = 1024 * 1024;
const int _dict = 4 * _1MB;	//字典大小4MB

namespace hfm{
	const HMFopreator compress = 1;
	const HMFopreator uncompress = 0;
}


struct fileHead {
	char fileIdentity[4] = "hFM";	//文件头标志
	unsigned int byteFrequency[256]{ 0 };	//存放频率
};


class HFMZipper {

	std::string path;
	size_t file_sz;
	fileHead head{ 0 };
	HFMTree* hfmtree;
	std::unordered_map<unsigned char, std::string> map;//字节码到hfm编码的映射
	std::unordered_map<std::string, unsigned char> map_re;//hfm编码到字节码的映射

	size_t getSize();
	size_t getSizeFromTree();	//树的根节点即是原始文件大小
	void calculateFrequency();	//读取文件计算各字节码频率
	void readFrequency();		//直接读取文件头得到频率
	void createHuffmanTree();	//根据文件头中byteFrequency生成Huffman树
	void getMap();				//根据Huffman树生成字节码到hfm编码的映射map
	void getMap_reverse();		//根据Huffman树生成hfm编码到字节码的映射map_re

public:
	HFMZipper(std::string& path, HMFopreator op = hfm::compress);
	~HFMZipper();
	bool doCompress();
	bool doUncompress();
	const HFMTree* getHFMTree() const;	//返回成员huffman
	void showMap() const;				//输出map
	void showMap_reverse() const;		//输出map_re
	void showHead() const;				//输出文件头中的频率

};
