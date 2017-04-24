#pragma once
#include "BTree.h"
#include <cstdlib> 

template <class ValueType>
class BNode
{
private:
	bool leaf;		// является ли узел листом
	int  nkeys;		// количество ключей узла
	int*  key;		// ключи узла
	ValueType *value;
	BNode<ValueType> **child;  // указатели на детей узла
	template <class ValueType>friend class BTree;

	BNode();
	~BNode();
	void traverse();
};

template <class ValueType>
BNode<ValueType>::BNode()
{
	leaf = true;
	nkeys = 0;
	//key = (int*)malloc(sizeof(int) * 2 * (BTree<ValueType>::t - 1));
	//value = (ValueType**)malloc(sizeof(ValueType*) * 2 * (BTree<ValueType>::t - 1));
	//child = (BNode<ValueType>**)malloc(sizeof(BNode<ValueType>*) * 2 * BTree<ValueType>::t);

	key = new int[2 * (BTree<ValueType>::t - 1)];
	value = new ValueType [2 * (BTree<ValueType>::t - 1)];
	child = new BNode<ValueType>* [2 * BTree<ValueType>::t];
}

template <class ValueType>
BNode<ValueType>::~BNode()
{
	//free(key);
	//free(value);
	//free(child);
}

template <class ValueType>
void BNode<ValueType>::traverse()
{
	int i;
	for (i = 0; i < nkeys; i++)
	{
		if (!leaf)
		{
			cout << endl;
			child[i]->traverse();
		}
		cout << " | " << key[i] << ":" << value[i];
	}
	if (!leaf)
	{
		cout << endl;
		child[i]->traverse();
	}
	cout << endl;
}