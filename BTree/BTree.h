#pragma once
#include "BNode.h"
#include "cstddef"

template <class ValueType>
class BTree
{
private:
	BNode<ValueType> * root;
	static const int t = 4;
public:
	BTree();
	BTree(int _t);
	static int getT();

	void insert(int key, ValueType& value);
	void traverse();
	bool search(int key, ValueType* _value = NULL);
	bool erase(int key);
	//void showHelper(BNode<ValueType> * node);

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


//TODO erase from habr
template<class ValueType>
bool BTree<ValueType>::erase(int _key)
{
	if (root == NULL) //Дерево пусто
	{
		return 0;
	}
	BNode<ValueType>* parent = NULL;
	BNode<ValueType>* cur = root;
	int i;
	while(true) // Поиск нужного элемента
	{
		i = 0;
		while (i < cur->nkeys && _key > cur->key[i])
		{
			i++;
		}
		if (cur->key[i] == _key) //Ключ найден
		{
			break;
		}
		if (cur->leaf) // Ключ не найден
		{
			return 0;
		}
		parent = cur;
		cur = cur->child[i];
	}
	if (cur->leaf)
	{
		if (cur->nkeys > t - 1) //Просто удаляем
		{

		}
		else
		{
			if() // Есть соседний
		}
	}


	return 1;
}


