#include "HFMZipper.h"
using namespace std;

HFMZipper::HFMZipper(string& _path, HMFopreator op) :path(_path)
{
	if(op) {
		getSize();	//��ȡ�ļ���С
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
{//��ȡ�ļ���С
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

	unsigned char* dict = new unsigned char[_dict];	//�ֵ䣬һ�δ����������
	int buffer_size = 0;
	string HMFcode = "";
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(unsigned char));
		buffer_size = fin.gcount();	//�ɹ���ȡ���ֽ���

		//�����ֽ�ͳ���ļ�������ִ�����ΪƵ��
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

	//�����ֽ��뵽HMFCode��ӳ��
	for (int i = 0; i < 256; ++i) {
		hfmtree->CreateHFMCode(HMFcode, i);
		map[i] = HMFcode;
	}
}

void HFMZipper::getMap_reverse()
{
	string HMFcode = "";

	//�����ֽ��뵽HMFCode��ӳ��
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

	unsigned char* dict = new unsigned char[_dict];	//�ֵ䣬һ�δ����������
	int buffer_size = 0;
	unsigned char ch = 0;	//��¼��λ�����ַ�
	unsigned char bitcount = 0;		//��¼��λ��ı���λ����

	//���ֽ�Ƶ��д���ļ�ͷ���Ա��ѹ
	fout.write((char*)&head, sizeof(fileHead));

	fin.seekg(0, ios::beg);
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(char));
		buffer_size = fin.gcount();	//�ɹ���ȡ���ֽ���

		for (int i = 0; i < buffer_size; ++i) {
			//�����ļ�ÿ���ֽ�
			for (char strcode : map[dict[i]]) {
				ch <<= 1;
				if (strcode == '1') {
					//��1���л�����ȼ������һλ��1
					ch |= 1;
				}
				bitcount++;//ÿ��λ��һ�ξͶ�bitcount++
				if (bitcount == 8) {
					//һ���ֽڷ���8������λ��д���ļ�������bitcount
					fout.write((char*)&ch, sizeof(unsigned char));//д��ch
					bitcount = 0;//����λ������0
				}
			}
		}
	} while (!fin.eof());

	//���ch���õı���λ����������Ϊ0�Ҳ���8�������油0д���ļ�
	if (bitcount > 0 && bitcount < 8) {
		ch <<= (8 - bitcount);
		fout.write((char*)&ch, sizeof(unsigned char));
	}
	
	////Ϊ�����ļ�ͷ��ЧbitΪ0��������̶�д�����һ�ֽڣ���Чλ��Ϊ0~7
	//ch <<= (8 - bitcount);
	//fout.write((char*)&ch, sizeof(unsigned char));

	//�����һλ��Чbit��д���ļ�ͷ
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

	unsigned char* dict = new unsigned char[_dict];	//�ֵ䣬һ�δ����������
	int buffer_size = 0;
	unsigned char ch = 0;	//��¼��λ�����ַ�
	unsigned char bitcount = 0;		//��¼��λ��ı���λ����
	int cur = hfmtree->getRoot();
	int size_uncompressed = 0;

	fin.seekg(sizeof(fileHead), ios::beg);
	do {
		memset(dict, 0, _dict);
		fin.read((char*)dict, _dict * sizeof(char));
		buffer_size = fin.gcount();	//�ɹ���ȡ���ֽ���

		for (int i = 0; i < buffer_size; ++i) {
			//�����ļ�ÿ���ֽ�
			ch = dict[i];
			bitcount = 0;
			while (bitcount < 8) {
				//����ǰλΪ1������Һ��ӣ�Ϊ0���������
				if (ch & 0x80)	//��1000 0000��λ��,��ǰλ1���1������Ϊ0
					cur = hfmtree->getRightChild(cur);
				else
					cur = hfmtree->getLeftChild(cur);

				if (hfmtree->getLeftChild(cur) == -1 && hfmtree->getRightChild(cur) == -1) {
					//�ߵ���Ҷ�ӽڵ㣬Ҷ�ӽڵ�����Ϊ0~255
					fout.write(((char*)&cur), sizeof(char));

					cur = hfmtree->getRoot();	//�ص����ڵ�

					size_uncompressed++;
					if (size_uncompressed == file_sz)
						break;	//��ѹ���С��ԭ�ļ���С��ͬ����ֹ
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

HFMTree* const HFMZipper::getHFMTree() const
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

