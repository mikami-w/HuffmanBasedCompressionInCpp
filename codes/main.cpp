#include"HFMZipper.h"
#include<iostream>
#include<fstream>
using namespace std;
int main()
{
	bool operation = 1;
	cout << "�����������\n[0]��ѹ�ļ�\n[1]ѹ���ļ�\n";
	cin >> operation;

	string _file;
	cout << "�������" << (operation ? "ѹ��" : "��ѹ") << "�����ļ����Ի����·����" << endl;
	cin >> _file;

	switch (operation) {
	case hfm::compress: {
		HFMZipper zipper(_file, hfm::compress);
		zipper.doCompress();
		break;
	}
	case hfm::uncompress: {
		HFMZipper unzipper(_file, hfm::uncompress);
		unzipper.doUncompress();
		break;
	}
	}

	//HFMZipper zipper(_file, compress);
	//zipper.doCompress();

	//_file += ".HFM";
	//HFMZipper unzipper(_file, uncompress);
	//unzipper.doUncompress();

	//����ļ�ͷ
	//cout << endl << "head1:" << endl;
	//zipper.showHead();
	//cout << endl << "head2:" << endl;
	//unzipper.showHead();

	//ǰ��������hfmtree
	//cout << endl << "preorder1:" << endl;
	//zipper.getHFMTree()->preOrder();
	//cout << endl << "preorder2:" << endl;
	//unzipper.getHFMTree()->preOrder();

	//����ֽ�����hfm����ӳ��
	//cout << endl << "map:" << endl;
	//zipper.showMap();
	//cout << endl << "map_reverse:" << endl;
	//unzipper.showMap_reverse();

	//ifstream fin("test.txt.txt");
	//unsigned char ch = 0;
	//cout << (unsigned int)'a' << endl;
	////while (!fin.eof()) {
	////	fin >> ch;
	////	cout << (int)ch<<' ';
	////}
	//fin >> ch;
	//cout << (unsigned int)ch << ' ';

	//unsigned int a[] = { 1,4,24,5,6,10,8,31,22,11 };
	//HFMTree tree(10,a);
	//tree.preOrder();

	return 0;
}