#pragma once
#include "BNode.h"
#include "cstddef"

template <class ValueType>
class BTree
{
private:
	BNode<ValueType> * root;
	static const int t = 3;
public:
	//BTree(int _t) : t(_t), root(NULL) {};
	BTree() : root(NULL) {};
	static int getT();
	template <class ValueType> friend class BNode;

	BNode<ValueType>* insert(int key, ValueType value);
	BNode<ValueType>* insertNonfull(BNode<ValueType> * node, int key, ValueType value);
	void splitNode(BNode<ValueType> * node, BNode<ValueType>* parent ,int index);

};

template <class ValueType>
int BTree<ValueType>::getT()
{
	return t;
};

template<class ValueType>
BNode<ValueType>* BTree<ValueType>::insert(int key, ValueType value)
{
	if (root == NULL)
	{
		root = new BNode<ValueType>;
		root->nkeys = 1;
		root->key[0] = key;
		root->value[0] = value;
		return root;
	}
	if (root->nkeys == 2 * (t - 1))
	{
		BNode<ValueType>* newroot = new BNode<ValueType>;
		newroot->leaf = false;
		newroot->child[0] = root;
		splitNode(root, newroot, 0);
		return insertNonfull(newroot, key, value);
	}
	return insertNonfull(root, key, value);
}

template<class ValueType>
BNode<ValueType>* BTree<ValueType>::insertNonfull(BNode<ValueType> * node, int key, ValueType value)
{
	int i;
	if (node->leaf)
	{
		for (i = node->nkeys - 1; i > 0 && key < node->key[i]; i--)
		{
			node->key[i + 1] = node->key[i];
		}
		node->key[i + 1] = key;
		node->nkeys++;
	}
	else
	{
		for (i = node->nkeys - 1; i > 0 && key < node->key[i]; i--);
		i++;
		if (node->child[i]->nkeys == 2 * (t - 1))
		{
			splitNode(node->child[i], node, i);
			if (key > node->key[i])
			{
				i++;
			}
		}
		node = insertNonfull(node->child[i], key, value);
	}
	return node;
}

template<class ValueType>
 void BTree<ValueType>::splitNode(BNode<ValueType>* node, BNode<ValueType>* parent, int index)
{
	int i;
	BNode<ValueType> * z = new BNode<ValueType>;
	z->leaf = node->leaf;
	z->nkeys = t - 1;
	for (i = 0; i < t; i++)
	{
		z->key[i] = node->key[t + i];
		z->value[i] = node->value[t + i];
	}
	if (!node->leaf)
	{
		for (i = 0; i < t; i++)
		{
			z->child[i] = node->child[t + i];
		}
	}
	node->nkeys = t - 1;

	for (i = parent->nkeys; i >= 0 && i <= index + 1; i--)
	{
		parent->child[i + 1] = parent->child[i];
	}
	parent->child[index + 1] = z;
	for (i = parent->nkeys - 1; i >= 0 && i <= index; i--)
	{
		parent->key[i + 1] = parent->key[i];
		parent->value[i + 1] = parent->value[i];
	}
	parent->key[index] = node->key[t - 1];
	parent->value[index] = node->value[t - 1];
	parent->nkeys++;
}