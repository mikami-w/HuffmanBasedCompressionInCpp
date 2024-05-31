#include "HFMZipper.h"
using namespace std;

HFMZipper::HFMZipper(string& _path, HMFopreator op) :path(_path)
{
	if(op) {
		getSize();	//获取文件大小
		calculateFrequency();
		createHuffmanTree();
		getMap();
	}
	else {
		readFrequency();
		createHuffmanTree();
		getMap_reverse();
		getSizeFromTree();
	}
}

HFMZipper::~HFMZipper()
{
	delete hfmtree;
}

size_t HFMZipper::getSize()
{//获取文件大小
	ifstream fin(path, ios::binary | ios::in);
	unsigned long long l, m;
	l = fin.tellg();
	fin.seekg(0, ios::end);
	m = fin.tellg();
	file_sz = m - l;	//bytes

	fin.close();
	return file_sz;
}

size_t HFMZipper::getSizeFromTree()
{
	file_sz = (*hfmtree)[hfmtree->getRoot()].weight;
	return file_sz;
}

void HFMZipper::calculateFrequency()
{
	ifstream fin(path, ios::binary | ios::in);
	if (!fin.is_open()) {
		cerr << "error in calculateFrequency";
		return;
	}

	unsigned char* dict = new unsigned char[_dict];	//字典，一次处理的数据量
	int buffer_size = 0;
	string HMFcode = "";
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(unsigned char));
		buffer_size = fin.gcount();	//成功读取的字节数

		//按照字节统计文件编码出现次数作为频率
		for (int i = 0; i < buffer_size; ++i) {
			++(head.byteFrequency[dict[i]]);
		}
	} while (!fin.eof());

	fin.close();
	delete[] dict;
}

void HFMZipper::readFrequency()
{
	ifstream fin(path, ios::binary | ios::in);
	//fin.open(path, ios::binary | ios::in);
	if (!fin.is_open()) {
		cerr << "error in readFrequency";
		return;
	}
	fin.read((char*)&head, sizeof(fileHead));
	fin.close();
}

void HFMZipper::createHuffmanTree()
{
	hfmtree = new HFMTree(nLeaf, head.byteFrequency);
}

void HFMZipper::getMap()
{
	string HMFcode = "";

	//建立字节码到HMFCode的映射
	for (int i = 0; i < 256; ++i) {
		hfmtree->CreateHFMCode(HMFcode, i);
		map[i] = HMFcode;
	}
}

void HFMZipper::getMap_reverse()
{
	string HMFcode = "";

	//建立字节码到HMFCode的映射
	for (int i = 0; i < 256; ++i) {
		hfmtree->CreateHFMCode(HMFcode, i);
		map_re[HMFcode] = i;
	}
}

bool HFMZipper::doCompress()
{
	ifstream fin(path, ios::binary | ios::in);
	if (!fin.is_open()) {
		cerr << "doCompress!fin.is_open()";
		return false;
	}
	ofstream fout(path + ".HFM", ios::binary | ios::trunc | ios::out);
	if (!fout.is_open()) {
		cerr << "doCompress!fout.is_open()";
		return false;
	}

	unsigned char* dict = new unsigned char[_dict];	//字典，一次处理的数据量
	int buffer_size = 0;
	unsigned char ch = 0;	//记录按位或后的字符
	unsigned char bitcount = 0;		//记录按位或的比特位长度

	//将字节频率写入文件头，以便解压
	fout.write((char*)&head, sizeof(fileHead));

	fin.seekg(0, ios::beg);
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(char));
		buffer_size = fin.gcount();	//成功读取的字节数

		for (int i = 0; i < buffer_size; ++i) {
			//遍历文件每个字节
			for (char strcode : map[dict[i]]) {
				ch <<= 1;
				if (strcode == '1') {
					//与1进行或操作等价于最后一位变1
					ch |= 1;
				}
				bitcount++;//每按位或一次就对bitcount++
				if (bitcount == 8) {
					//一个字节放满8个比特位就写入文件并重置bitcount
					fout.write((char*)&ch, sizeof(unsigned char));//写入ch
					bitcount = 0;//比特位计数置0
				}
			}
		}
	} while (!fin.eof());

	//检测ch放置的比特位个数，若不为0且不够8个，后面补0写入文件
	if (bitcount > 0 && bitcount < 8) {
		ch <<= (8 - bitcount);
		fout.write((char*)&ch, sizeof(unsigned char));
	}
	
	////为适配文件头有效bit为0的情况，固定写入最后一字节，有效位数为0~7
	//ch <<= (8 - bitcount);
	//fout.write((char*)&ch, sizeof(unsigned char));

	//将最后一位有效bit数写入文件头
	//fout.seekp(0, ios::beg);
	//fout.write((char*)&bitcount, sizeof(unsigned char));

	fin.close();
	fout.close();
	delete[] dict;
	return true;
}

bool HFMZipper::doUncompress()
{
	ifstream fin(path, ios::binary | ios::in);
	if (!fin.is_open()) {
		cerr << "doUncompress!fin.is_open()";
		return false;
	}
	ofstream fout(path + ".UNHFM", ios::binary | ios::trunc | ios::out);
	if (!fout.is_open()) {
		cerr << "doUncompress!fout.is_open()";
		return false;
	}

	unsigned char* dict = new unsigned char[_dict];	//字典，一次处理的数据量
	int buffer_size = 0;
	unsigned char ch = 0;	//记录按位或后的字符
	unsigned char bitcount = 0;		//记录按位或的比特位长度
	int cur = hfmtree->getRoot();
	int size_uncompressed = 0;

	fin.seekg(sizeof(fileHead), ios::beg);
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(char));
		buffer_size = fin.gcount();	//成功读取的字节数

		for (int i = 0; i < buffer_size; ++i) {
			//遍历文件每个字节
			ch = dict[i];
			bitcount = 0;
			while (bitcount < 8) {
				//若当前位为1则访问右孩子，为0则访问左孩子
				if (ch & 0x80)	//与1000 0000按位与,当前位1则得1，否则为0
					cur = hfmtree->getRightChild(cur);
				else
					cur = hfmtree->getLeftChild(cur);

				if (hfmtree->getLeftChild(cur) == -1 && hfmtree->getRightChild(cur) == -1) {
					//走到了叶子节点，叶子节点索引为0~255
					fout.write(((char*)&cur), sizeof(char));

					cur = hfmtree->getRoot();	//回到根节点

					size_uncompressed++;
					if (size_uncompressed == file_sz)
						break;	//解压后大小与原文件大小相同，终止
				}
				bitcount++;
				ch <<= 1;
			}
		}
	} while (!fin.eof());

	fin.close();
	fout.close();
	delete[] dict;
	return true;
}

const HFMTree* HFMZipper::getHFMTree() const
{
	return hfmtree;
}

void HFMZipper::showMap() const
{
	for (auto x : map) {
		cout.width(3);
		cout << (unsigned int)x.first << ": " << x.second << '\n';
	}
}

void HFMZipper::showMap_reverse() const
{
	for (auto x : map_re) {
		cout.width(3);
		cout << (unsigned int)x.second << ": " << x.first << '\n';
	}
}

void HFMZipper::showHead() const
{
	for (auto x : head.byteFrequency) {
		cout << x << ' ';
	}
}

