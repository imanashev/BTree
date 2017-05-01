#pragma once
#include "BTree.h"
#include <cstdlib> 
//#define  T BTree<V>::t;

template <class V>
class BNode
{
private:
	bool leaf;
	int  nkeys;
	int*  key;
	V *value;
	BNode<V> **child;

	BNode();
	~BNode();
	void insertNonfull(int _key, V _value);
	void splitNode(BNode<V> * node, int index);

	//V eraseSrc(int _key);
	//void erase(BNode<V> * parent, int index);

	void mergeNode(BNode<V> * node, int index);
	void moveNode(BNode<V> * node, int index);

	void traverse(int tab = 0);
	bool search(int _key, V* _value);

	template <class V>friend class BTree;
};

template <class V>
BNode<V>::BNode()
{
	leaf = true;
	nkeys = 0;
	//key = (int*)malloc(sizeof(int) * 2 * (BTree<V>::t - 1));
	//value = (V**)malloc(sizeof(V*) * 2 * (BTree<V>::t - 1));
	//child = (BNode<V>**)malloc(sizeof(BNode<V>*) * 2 * BTree<V>::t);

	key = new int[2 * BTree<V>::t - 1];
	value = new V [2 * BTree<V>::t - 1];
	child = new BNode<V>* [2 * BTree<V>::t];
}

template <class V>
BNode<V>::~BNode()
{
	//free(key);
	//free(value);
	//free(child);
}

template <class V>
void BNode<V>::insertNonfull(int _key, V _value)
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
		if (child[i]->nkeys == 2 * BTree<V>::t - 1)
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

template <class V>
void BNode<V>::splitNode(BNode<V>* node, int index)
{
	int i;
	BNode<V> * z = new BNode<V>;
	z->leaf = node->leaf;
	z->nkeys = BTree<V>::t - 1;
	for (i = 0; i < BTree<V>::t - 1; i++)
	{
		z->key[i] = node->key[BTree<V>::t + i];
		z->value[i] = node->value[BTree<V>::t + i];
	}
	if (!node->leaf)
	{
		for (i = 0; i < BTree<V>::t; i++)
		{
			z->child[i] = node->child[BTree<V>::t + i];
		}
	}
	node->nkeys = BTree<V>::t - 1;

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
	key[index] = node->key[BTree<V>::t - 1];
	value[index] = node->value[BTree<V>::t - 1];
	nkeys++;
}

template <class V>
void BNode<V>::traverse(int tab)
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

template <class V>
bool BNode<V>::search(int _key, V* _value)
{
	int i = 0;
	while (i < nkeys && _key > key[i])
	{
		i++;
	}
	if (key[i] == _key)
	{
		if (_value)
		{
			*_value = value[i];
		}
		return 1;
	}
	if (leaf)
	{
		return 0;
	}
	return child[i]->search(_key, _value);
}


//template<class V>
//void BNode<V>::moveNode(BNode<V>* parent, int index)
//{
//	if (parent->child[index + 1] >= BTree<V>::t)
//	{
//
//	}
//	else if (parent->child[index - 1] >= BTree<V>::t)
//	{
//
//	}
//}

//template <class V>
//bool BNode<V>::eraseSrc(int _key)
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
//template <class V>
//void BNode<V>::erase(BNode<V> * parent,int index)
//{
//	if (leaf)
//	{
//		if (nkeys > BTree<V>::t - 1)
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
