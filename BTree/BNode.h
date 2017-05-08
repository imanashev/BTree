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

	int findKey(int _key);
	bool remove(int _key);
	bool removeFromLeaf(int idx);
	bool removeFromNonLeaf(int idx);
	void getPred(int idx, int* _key, V* _value);
	void getSucc(int idx, int* _key, V* _value);
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
int BNode<V>::findKey(int _key)
{
	int idx = 0;
	while (idx < nkeys && key[idx] < _key)
		++idx;
	return idx;
}

template<class V>
bool BNode<V>::remove(int _key)
{
	int idx = findKey(_key);

	if (idx < nkeys && key[idx] == _key)
	{
		if (leaf)
		{
			return removeFromLeaf(idx);
		}
		else
		{
			return removeFromNonLeaf(idx);
		}
	}
	else
	{
		if (leaf)
		{
			return false;
		}

		bool flag = (idx == nkeys);

		if (child[idx]->nkeys < BTree<V>::t)
		{
			fill(idx);
		}

		if (flag && idx > nkeys)
		{
			return child[idx - 1]->remove(_key);
		}
		else
		{
			return child[idx]->remove(_key);
		}
	}
}

template<class V>
bool BNode<V>::removeFromLeaf(int idx)
{
	for (int i = idx + 1; i < nkeys; i++)
	{
		key[i - 1] = key[i];
		value[i - 1] = value[i];
	}
	nkeys--;
	return true;
}

//TODO
template<class V>
bool BNode<V>::removeFromNonLeaf(int idx)
{

	int k = key[idx];

	if (child[idx]->nkeys >= BTree<V>::t)
	{
		int k;
		V v;
		getPred(idx, &k, &v);
		key[idx] = k;
		value[idx] = &v;
		return child[idx]->remove(key[idx]);
	}

	else if (child[idx + 1]->nkeys >= BTree<V>::t)
	{
		int k;
		V v;
		getSucc(idx, &k, &v);
		key[idx] = k;
		value[idx] = &v;
		return child[idx + 1]->remove(key[idx]);
	}

	else
	{
		merge(idx);
		return child[idx]->remove(k);
	}

}

//TODO
template<class V>
void BNode<V>::getPred(int idx, int* _key, V* _value)
{
	BNode<V> *cur = child[idx];

	while (!cur->leaf)
	{
		cur = cur->child[cur->nkeys];
	}

	_key = cur->key[cur->nkeys - 1];
	_value = cur->value[cur->nkeys - 1]
}

//TODO
template<class V>
void BNode<V>::getSucc(int idx, int* _key, V* _value)
{
	BNode<V> *cur = child[idx + 1];

	while (!cur->leaf)
	{
		cur = cur->child[0];
	}
	
	_key = cur->key[0];
	_value = cur->value[0];
}

template<class V>
void BNode<V>::fill(int idx)
{
	if (idx != 0 && child[idx - 1]->nkeys >= BTree<V>::t)
	{
		borrowFromPrev(idx);
	}

	else if (idx != nkeys && child[idx + 1]->nkeys >= BTree<V>::t)
	{
		borrowFromNext(idx);
	}

	else
	{
		if (idx != nkeys)
		{
			merge(idx);
		}
		else
		{
			merge(idx - 1);
		}
	}
}

template<class V>
void BNode<V>::borrowFromPrev(int idx)
{
	BNode<V>* node = child[idx];
	BNode<V>* sibling = child[idx - 1];

	for (int i = node->nkeys - 1; i >= 0; i--)
	{
		node->key[i + 1] = node->key[i];
		node->value[i + 1] = node->value[i];
	}

	if (!node->leaf)
	{
		for (int i = node->nkeys; i >= 0; i--)
		{
			node->child[i + 1] = node->child[i];
		}
	}

	node->key[0] = key[idx - 1];

	if (!leaf)
	{
		node->child[0] = sibling->child[sibling->nkeys];
	}

	key[idx - 1] = sibling->key[sibling->nkeys - 1];
	value[idx - 1] = sibling->value[sibling->nkeys - 1];

	node->nkeys += 1;
	sibling->nkeys -= 1;
}

template<class V>
void BNode<V>::borrowFromNext(int idx)
{
	BNode<V>* node = child[idx];
	BNode<V>* sibling = child[idx + 1];

	node->key[node->nkeys] = key[idx];
	node->value[node->nkeys] = value[idx];

	if (!(node->leaf))
	{
		node->child[(node->n) + 1] = sibling->child[0];
	}

	key[idx] = sibling->key[0];
	value[idx] = sibling->value[0];

	for (int i = 1; i < sibling->nkeys; i++)
	{
		sibling->key[i - 1] = sibling->key[i];
		sibling->value[i - 1] = sibling->value[i];
	}

	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
		{
			sibling->child[i - 1] = sibling->child[i];
		}
	}

	node->nkeys += 1;
	sibling->nkeys -= 1;
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