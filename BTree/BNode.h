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
	ValueType **value;
	BNode<ValueType> **child;  // ��������� �� ����� ����
	template <class ValueType>friend class BTree;

	BNode();
	~BNode();
};

template <class ValueType>
BNode<ValueType>::BNode()
{
	leaf = true;
	nkeys = 0;
	key = (int*)malloc(sizeof(int) * 2 * (BTree<ValueType>::t - 1));
	value = (ValueType**)malloc(sizeof(ValueType*) * 2 * (BTree<ValueType>::t - 1));
	child = (BNode<ValueType>**)malloc(sizeof(BNode<ValueType>*) * 2 * BTree<ValueType>::t);
}


template <class ValueType>
BNode<ValueType>::~BNode()
{
	free(key);
	free(value);
	free(child);
}