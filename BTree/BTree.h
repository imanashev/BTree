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

	void insert(int key, ValueType& value);
	BNode<ValueType>* insertNonfull(BNode<ValueType> * node, int key, ValueType value);
	void splitNode(BNode<ValueType> * node, BNode<ValueType>* parent ,int index);

	void show();
	void traverse();
	void showHelper(BNode<ValueType> * node);

};

template<class ValueType>
void BTree<ValueType>::show()
{
	showHelper(root);
}

template<class ValueType>
void BTree<ValueType>::traverse()
{
	if (root != NULL)
	{
		root->traverse();
	}
}

 template<class ValueType>
 void BTree<ValueType>::showHelper(BNode<ValueType>* node)
 {
	 for (int i = 0; i < node->nkeys; i++)
	 {
		 cout << node->key[i] << " : " << (node->value[i]) << ";" << endl;
	 }
	 if (!node->leaf)
	 {
		 for (int i = 0; i < node->nkeys; i++)
		 {
			 cout << endl << "go to leaf " << i << endl;
			 showHelper(node->child[i]);
		 }
	 }

 }

template <class ValueType>
int BTree<ValueType>::getT()
{
	return t;
};

template<class ValueType>
void BTree<ValueType>::insert(int key, ValueType& value)
{
	if (root == NULL)
	{
		root = new BNode<ValueType>;
		root->nkeys = 1;
		root->leaf = true;
		root->key[0] = key;
		root->value[0] = value;
	}
	else
	{

		if (root->nkeys == 2 * (t - 1))
		{
			BNode<ValueType>* newroot = new BNode<ValueType>;
			newroot->leaf = false;
			newroot->nkeys = 0;
			newroot->child[0] = root;
			splitNode(root, newroot, 0);
			int i = 0;
			if (newroot->key[0] < key)
			{
				i++;
			}
			insertNonfull(newroot->child[i], key, value);
			root = newroot;
		}
		else
		{
			insertNonfull(root, key, value);
		}
	}
}

template<class ValueType>
BNode<ValueType>* BTree<ValueType>::insertNonfull(BNode<ValueType> * node, int key, ValueType value)
{
	int i = node->nkeys - 1;
	if (node->leaf)
	{
		while( i > 0 && node->key[i] > key)
		{
			node->key[i + 1] = node->key[i];
			node->value[i + 1] = node->value[i];
			i--;
		}
		node->key[i + 1] = key;
		node->value[i + 1] = value;
		node->nkeys++;
	}
	else
	{
		while( i > 0 && node->key[i] > key) 
		{
			i--;
		}
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
	for (i = 0; i < t-1; i++)
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

	for (i = parent->nkeys;  i >= index + 1; i--) //i >= 0 && i <= index + 1; i--)
	{
		parent->child[i + 1] = parent->child[i];
	}
	parent->child[index + 1] = z;
	for (i = parent->nkeys - 1; i >= index; i--) //i >= 0 && i <= index; i--)
	{
		parent->key[i + 1] = parent->key[i];
		parent->value[i + 1] = parent->value[i];
	}
	parent->key[index] = node->key[t - 1];
	parent->value[index] = node->value[t - 1];
	parent->nkeys++;
}