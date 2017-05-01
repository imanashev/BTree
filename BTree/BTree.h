#pragma once
#include "BNode.h"
#include "cstddef"

template <class V>
class BTree
{
private:
	BNode<V> * root;
	static const int t = 4;
public:
	BTree();
	BTree(int _t);
	static int getT();

	void insert(int key, V& value);
	void traverse();
	bool search(int key, V* _value = NULL);
	bool erase(int key);
	//void showHelper(BNode<V> * node);

	template <class V> friend class BNode;
};

template<class V>
inline BTree<V>::BTree() : root(NULL) {};

template<class V>
inline BTree<V>::BTree(int _t) : t(_t), root(NULL) {};

template <class V>
int BTree<V>::getT()
{
	return t;
};

template<class V>
void BTree<V>::insert(int key, V& value)
{
	if (root == NULL)
	{
		root = new BNode<V>;
		root->nkeys = 1;
		root->leaf = true;
		root->key[0] = key;
		root->value[0] = value;
	}
	else
	{
		if (root->nkeys == 2 * t - 1)
		{
			BNode<V>* newroot = new BNode<V>;
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

template<class V>
void BTree<V>::traverse()
{
	if (root != NULL)
	{
		root->traverse();
	}
}

template<class V>
bool BTree<V>::search(int key, V* _value)
{
	if (root != NULL)
	{
		return root->search(key , _value);
	}
	return 0;
}


//TODO erase from habr
template<class V>
bool BTree<V>::erase(int _key)
{
	if (root == NULL) //������ �����
	{
		return 0;
	}
	/* ����� ������ */
	BNode<V>* parent = NULL;
	BNode<V>* node = root;
	int nodeNo; // ������ ���������� ��������
	int parentNo; // ������ node � parent
	while(true) // ����� ������� ��������
	{
		nodeNo = 0;
		while (nodeNo < node->nkeys && _key > node->key[nodeNo])
		{
			nodeNo++;
		}
		if (node->key[nodeNo] == _key) //���� ������
		{
			break;
		}
		if (node->leaf) // ���� �� ������
		{
			return 0;
		}
		parent = node;
		parentNo = nodeNo;
		node = node->child[nodeNo];
	}

	/* ����� �������� */
	if (node->leaf) // �� �����
	{
		if (node->nkeys > t - 1) //������ ������ t-1: ������ �������
		{
			delete node->value[nodeNo];

			for (int i = nodeNo; i < node->nkeys - 1; i++)
			{
				node->key[i] = node->key[i + 1];
				node->value[i] = node->value[i + 1];
			}
			node->nkeys--;
		}
		else // ������ ������ t-1
		{
			bool first, last;
			last = parentNo != parent->nkeys - 1;
			first = parentNo != 0;
			BNode<V>* neighbor = NULL;

			// ���� �������� c nkeys > t-1
			if (!last)
			{
				if (parent->child[parentNo + 1]->nkeys > t - 1) // ����� ������
				{
					/* ���� ���������� �������� ���� (����������� ����� � ���
					� ������� ������ �� ��������), ������� �������� ������ t-1
					�����, �� ������� ���� �� ����� ������, ������� ��������
					������������ ����� ����������� ������� ����-������ �
					��������� ���� (�� ���� �� ������ ���� �� ����� ������ �
					�� ������ ���� �� ������). ����� ��� ���� k1. �������
					���� k2 �� ����-��������, ������� �������� ������������
					��������� ���� � ��� ������, ������� �� ������� �����.
					������ �� ��������� ���� ������ ���� (������� ����������
					���� �������), ������� k2 � ���� ����, � ������ k2 �
					����-�������� �������� k1.
					*/
					neighbor = parent->child[parentNo + 1];

					delete node->value[nodeNo];
					for (int i = nodeNo; i < node->nkeys - 1; i++)
					{
						node->key[i] = node->key[i + 1];
						node->value[i] = node->value[i + 1];
					}
					node->key[nkeys - 1] = parent->key[parentNo];
					node->value[nkeys - 1] = parent->value[parentNo];

					parent->key[parentNo] = neighbor->key[0];
					parent->value[parentNo] = neighbor->value[0];

					for (int i = 0; i < neighbor->nkeys - 1; i++)
					{
						neighbor->key[i] = neighbor->key[i + 1];
						neighbor->key[i] = neighbor->key[i + 1];
					}
					neighbor->nkeys--;
					first = 1;
				}
			}
			if (!first)
			{
				if (parent->child[parentNo - 1]->nkeys > t - 1) // ����� �����
				{
					neighbor = parent->child[parentNo - 1];

					delete node->value[nodeNo];
					for (int i = nodeNo; i > 0; i--)
					{
						node->key[i] = node->key[i - 1];
						node->value[i] = node->value[i - 1];
					}
					node->key[0] = parent->key[parentNo];
					node->value[0] = parent->value[parentNo];

					parent->key[parentNo] = neighbor->key[nkeys - 1];
					parent->value[parentNo] = neighbor->value[nkeys - 1];

					neighbor->nkeys--;
				}
			}
			if (neighbor) // ��� ������ �� t-1 �����
			{
				/* ���� �� ��� ������ ������ ���� ����� �� t-1 �����.
				�� �� ���������� ��� � �����-���� �������, ������� ������
				����. � ��� ���� �� ����-��������, ������� ��� ������������
				��� ���� ���� �������� �������, ���������� � ���
				������������������ ���� (��������, �� ����� � ��� ��������).
				*/
				if (parentNo == 0) // ���������� � ������ �������
				{
					///TODO
				}
				else // ���������� � ����� �������
				{
					///TODO
				}
			}
		}
	}
	else // �� ����
	{
		if () // ���� �������� ����, �������������� ����� k �������� ������ t-1 �����
		{
			/* �� ������� k1 � ��������������� k � ��������� ����� ����. 
			������� ��� (���������� ��������� ��� ��������). �������� k 
			� �������� ���� �� k1. 
			
			����������� ����������� ������, ���� �������� ����, ��������� �� ������ k,
			����� ������ t-1 �����.
			*/
		}
		else if () //���� ��� (��������� � �������������� �������� ����) ����� �� t-1 �����,
		{
			/*�� ���������� ���� �����, ��������� � ��� k, � ����� ������� k �� ������
			����(���������� ��������� ��� ��������).*/
		}
		else if () //���� ��������� 2 ��������� ������� �����
		{
			/*�� ��� ���������� ������,	� ���������� ������ �������������.
			*/
		}

	}


	return 1;
}


