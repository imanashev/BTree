#pragma once
#include "BTree.h"
#include <cstdlib> 
//#define  T BTree<ValueType>::t;

template <class ValueType>
class BNode
{
private:
	bool leaf;
	int  nkeys;
	int*  key;
	ValueType *value;
	BNode<ValueType> **child;

	BNode();
	~BNode();
	void insertNonfull(int _key, ValueType _value);
	void splitNode(BNode<ValueType> * node, int index);

	//ValueType eraseSrc(int _key);
	//void erase(BNode<ValueType> * parent, int index);

	void mergeNode(BNode<ValueType> * node, int index);
	void moveNode(BNode<ValueType> * node, int index);

	void traverse(int tab = 0);
	bool search(int _key, ValueType* _value);

	template <class ValueType>friend class BTree;
};

template <class ValueType>
BNode<ValueType>::BNode()
{
	leaf = true;
	nkeys = 0;
	//key = (int*)malloc(sizeof(int) * 2 * (BTree<ValueType>::t - 1));
	//value = (ValueType**)malloc(sizeof(ValueType*) * 2 * (BTree<ValueType>::t - 1));
	//child = (BNode<ValueType>**)malloc(sizeof(BNode<ValueType>*) * 2 * BTree<ValueType>::t);

	key = new int[2 * BTree<ValueType>::t - 1];
	value = new ValueType [2 * BTree<ValueType>::t - 1];
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
void BNode<ValueType>::insertNonfull(int _key, ValueType _value)
{
	int i = nkeys - 1;
	if (leaf)
	{
		while (i >= 0 && key[i] > _key)
		{
			key[i + 1] = key[i];
			value[i + 1] = value[i];
			i--;
		}
		key[i + 1] = _key;
		value[i + 1] = _value;
		nkeys++;
	}
	else
	{
		while (i >= 0 && key[i] > _key)
		{
			i--;
		}
		i++;
		if (child[i]->nkeys == 2 * BTree<ValueType>::t - 1)
		{
			splitNode(child[i], i);
			if (_key > key[i])
			{
				i++;
			}
		}
		child[i]->insertNonfull(_key, _value);
	}
}

template <class ValueType>
void BNode<ValueType>::splitNode(BNode<ValueType>* node, int index)
{
	int i;
	BNode<ValueType> * z = new BNode<ValueType>;
	z->leaf = node->leaf;
	z->nkeys = BTree<ValueType>::t - 1;
	for (i = 0; i < BTree<ValueType>::t - 1; i++)
	{
		z->key[i] = node->key[BTree<ValueType>::t + i];
		z->value[i] = node->value[BTree<ValueType>::t + i];
	}
	if (!node->leaf)
	{
		for (i = 0; i < BTree<ValueType>::t; i++)
		{
			z->child[i] = node->child[BTree<ValueType>::t + i];
		}
	}
	node->nkeys = BTree<ValueType>::t - 1;

	for (i = nkeys; i >= index + 1; i--)
	{
		child[i + 1] = child[i];
	}
	child[index + 1] = z;
	for (i = nkeys - 1; i >= index; i--)
	{
		key[i + 1] = key[i];
		value[i + 1] = value[i];
	}
	key[index] = node->key[BTree<ValueType>::t - 1];
	value[index] = node->value[BTree<ValueType>::t - 1];
	nkeys++;
}

template <class ValueType>
void BNode<ValueType>::traverse(int tab)
{
	int i;
	for (i = 0; i < nkeys; i++)
	{
		if (!leaf)
		{
			child[i]->traverse(tab + 1);
		}
		for (int j = 0; j < tab; j++) cout << "	";
		cout << "[" << key[i] << "]:" << value[i];
		cout << endl;
	}

	if (!leaf)
	{
		child[i]->traverse(tab + 1);
	}
}

template <class ValueType>
bool BNode<ValueType>::search(int _key, ValueType* _value)
{
	int i = 0;
	while (i < nkeys && _key > key[i])
	{
		i++;
	}
	if (key[i] == _key)
	{
		*_value = value[i];
		return 1;
	}
	if (leaf)
	{
		return 0;
	}
	return child[i]->search(_key, _value);
}


//template<class ValueType>
//void BNode<ValueType>::moveNode(BNode<ValueType>* parent, int index)
//{
//	if (parent->child[index + 1] >= BTree<ValueType>::t)
//	{
//
//	}
//	else if (parent->child[index - 1] >= BTree<ValueType>::t)
//	{
//
//	}
//}

//template <class ValueType>
//bool BNode<ValueType>::eraseSrc(int _key)
//{
//	int i = 0;
//	while (i < nkeys && _key > key[i])
//	{
//		i++;
//	}
//	if (key[i] == _key)
//	{
//		erase();
//		return 1 
//	}
//	if (leaf)
//	{
//		return 0;
//	}
//	return child[i]->erase(_key);
//}
//
//template <class ValueType>
//void BNode<ValueType>::erase(BNode<ValueType> * parent,int index)
//{
//	if (leaf)
//	{
//		if (nkeys > BTree<ValueType>::t - 1)
//		{
//			for (int i = index; i < nkeys - 1; i++)
//			{
//				key[i] = key[i + 1];
//				value[i] = value[i + 1];
//			}
//			nkeys--;
//		}
//		else
//		{
//
//		}
//	}
//	else
//	{
//
//	}
//
//
//}
