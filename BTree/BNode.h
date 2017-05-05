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
	V **value;
	BNode<V> **child;

protected:
	BNode();
	~BNode();

	void insertNonfull(int _key, V* _value);
	void splitNode(BNode<V> * node, int idx);

	void traverse(int tab = 0);
	bool search(int _key, V* _value);

	bool remove(int key);
	bool removeFromLeaf(int idx);
	bool removeFromNonLeaf(int idx);
	int getPred(int idx);
	int getSucc(int idx);
	void fill(int idx);
	void borrowFromPrev(int idx);
	void borrowFromNext(int idx);
	void merge(int idx);

	template <class V>friend class BTree;
};

template <class V>
BNode<V>::BNode()
{
	leaf = true;
	nkeys = 0;
	
	key = new int[2 * BTree<V>::t - 1];
	value = new V* [2 * BTree<V>::t - 1];
	child = new BNode<V>* [2 * BTree<V>::t];
}


template <class V>
void BNode<V>::insertNonfull(int _key, V* _value)
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
void BNode<V>::splitNode(BNode<V>* node, int idx)
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

	for (i = nkeys; i >= idx + 1; i--)
	{
		child[i + 1] = child[i];
	}
	child[idx + 1] = z;
	for (i = nkeys - 1; i >= idx; i--)
	{
		key[i + 1] = key[i];
		value[i + 1] = value[i];
	}
	key[idx] = node->key[BTree<V>::t - 1];
	value[idx] = node->value[BTree<V>::t - 1];
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
		cout << "[" << key[i] << "]:" << *value[i];
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
			*_value = *value[i];
		}
		return 1;
	}
	if (leaf)
	{
		return 0;
	}
	return child[i]->search(_key, _value);
}

//TODO
template<class V>
void BNode<V>::borrowFromNext(int idx)
{
	BNode<V>* child = child[idx];
	BNode<V>* sibling = child[idx + 1];

	
}

template<class V>
void BNode<V>::merge(int idx)
{
	BNode<V>* newNode = parent->child[idx];
	BNode<V>* oldNode = parent->child[idx + 1];

	// Дописываем медиану
	newNode->key[newNode->nkeys] = parent->key[idx];
	newNode->value[newNode->nkeys] = parent->value[idx];
	newNode->nkeys++;

	// Дописываем соседа
	for (int i = 0; i < oldNode->nkeys; i++)
	{
		newNode->key[newNode->nkeys + i] = oldNode->key[i];
		newNode->value[newNode->nkeys + i] = oldNode->value[i];
	}
	if (!newNode->leaf)
	{
		for (int i = 0; i <= oldNode->nkeys; i++)
		{
			newNode->child[newNode->nkeys + i] = oldNode->child[i];
		}
	}
	newNode->nkeys += oldNode->nkeys;

	// Изменяем родителя
	for (int i = idx + 1; i < nkeys; i++)
	{
		key[i - 1] = key[i];
		value[i - 1] = value[i];
	}
	for (int i = idx + 2; i <= nkeys; i++)
	{
		child[i - 1] = child[i];
	}
	nkeys--;
}


