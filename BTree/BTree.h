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
	BTree();
	BTree(int _t);
	static int getT();

	void insert(int key, ValueType& value);
	void traverse();
	bool search(int key, ValueType* _value);
	bool erase(int key);
	void showHelper(BNode<ValueType> * node);

	template <class ValueType> friend class BNode;
};

template<class ValueType>
inline BTree<ValueType>::BTree() : root(NULL) {};

template<class ValueType>
inline BTree<ValueType>::BTree(int _t) : t(_t), root(NULL) {};

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
		if (root->nkeys == 2 * t - 1)
		{
			BNode<ValueType>* newroot = new BNode<ValueType>;
			newroot->leaf = false;
			newroot->nkeys = 0;
			newroot->child[0] = root;
			newroot->splitNode(root, 0);
			int i = 0;
			if (newroot->key[0] < key)
			{
				i++;
			}
			newroot->child[i]->insertNonfull(key, value);
			root = newroot;
		}
		else
		{
			root->insertNonfull(key, value);
		}
	}
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
bool BTree<ValueType>::search(int key, ValueType* _value)
{
	if (root != NULL)
	{
		return root->search(key , _value);
	}
	return 0;
}

//template<class ValueType>
//ValueType BTree<ValueType>::erase(int key)
//{
//	if (root != NULL)
//	{
//		return root->erase(key);
//	}
//	return 0;
//}

