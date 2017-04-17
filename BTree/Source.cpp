#include <iostream>
#include <string>
#include "BTree.h"

using namespace std;

void main()
{
	BTree<string> tree;
	/*tree.insert(11, "hello");
	tree.insert(13, "dsdf");
	tree.insert(12, "dsfdsfsd");*/
	string a;
	for (int i = 0; i < 4; i++)
	{
		a = "heldsffffffffffffffffffffffffffflo" + (char)i;
		tree.insert(i, a);
	}
	tree.show();

	/*BTree<int> tree1;
	tree1.insert(11, 3);
	tree1.insert(13, 2);
	tree1.insert(12, 4);*/
}