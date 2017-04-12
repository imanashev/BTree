#pragma once
#include "BTree.h"
#include <cstdlib> 

template <class ValueType>
class BNode
{
private:
	bool leaf;		// �������� �� ���� ������
	int  nkeys;		// ���������� ������ ����
	int*  key;		// ����� ����
	ValueType* value;
	BNode<ValueType>** child;  // ��������� �� ����� ����
	template <class ValueType>friend class BTree;

	BNode();
	~BNode();
};

template <class ValueType>
BNode<ValueType>::BNode()
{
	leaf = true;
	nkeys = 0;
	key = (int*)malloc(sizeof(this->key) * 2 * (BTree<ValueType>::t - 1));
	value = (ValueType*)malloc(sizeof(this->value) * 2 * (BTree<ValueType>::t - 1));
	child = (BNode<ValueType>**)malloc(sizeof(this->child) * 2 * BTree<ValueType>::t);


}

template <class ValueType>
BNode<ValueType>::~BNode()
{
	free(key);
	free(value);
	free(child);
}