#include"HFMZipper.h"
#include<iostream>
#include<fstream>
using namespace std;
int main()
{
	bool operation = 1;
	cout << "请输入操作：\n[0]解压文件\n[1]压缩文件\n";
	cin >> operation;

	string _file;
	cout << "请输入待" << (operation ? "压缩" : "解压") << "处理文件绝对或相对路径：" << endl;
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

	//输出文件头
	//cout << endl << "head1:" << endl;
	//zipper.showHead();
	//cout << endl << "head2:" << endl;
	//unzipper.showHead();

	//前序遍历输出hfmtree
	//cout << endl << "preorder1:" << endl;
	//zipper.getHFMTree()->preOrder();
	//cout << endl << "preorder2:" << endl;
	//unzipper.getHFMTree()->preOrder();

	//输出字节码与hfm编码映射
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