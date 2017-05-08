#pragma once
#include "BNode.h"
#include "cstddef"

template <class V>
class BTree
{
	template <class V> friend class BNode;
	typedef void(*FunctionType)(V& anitem);
private:
	BNode<V> * root;
	static const int t;
public:
	BTree();
	BTree(int _t);
	int getT();

	void insert(int key, V* value);
	bool remove(int key);

	void traverse(FunctionType visit);
	void print();
	bool search(int key, V* _value = NULL);
};

template<class V>
inline BTree<V>::BTree() : root(NULL) {};

template<class V>
inline BTree<V>::BTree(int _t) : t(_t), root(NULL) {};

template <class V>
inline int BTree<V>::getT()
{
	return t;
}

template<class V>
void BTree<V>::insert(int key, V* value)
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
bool BTree<V>::remove(int key)
{
	if (!root) //Дерево пусто
	{
		return false;
	}

	bool result = root->remove(key);

	if (root->nkeys == 0) // Корень стал пустым
	{
		BNode<V> *tmp = root;
		if (root->leaf)
		{
			root = NULL;
		}
		else
		{
			root = root->child[0];
		}
		delete tmp;
	}
	return result;
}

template<class V>
void BTree<V>::traverse(FunctionType visit)
{
	if (root != NULL)
	{
		root->traverse(visit);
	}
}

template<class V>
void BTree<V>::print()
{
	if (root != NULL)
	{
		root->print();
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




//template<class V>
//bool BTree<V>::erase(int _key)
//{
//	if (root == NULL) //Дерево пусто
//	{
//		return 0;
//	}
//	/* Часть поиска */
//	BNode<V>* parent = NULL;
//	BNode<V>* node = root;
//	int nodeNo; // Индекс удаляемого элемента
//	int parentNo; // Индекс node в parent
//	while(true) // Поиск нужного элемента
//	{
//		nodeNo = 0;
//		while (nodeNo < node->nkeys && _key > node->key[nodeNo])
//		{
//			nodeNo++;
//		}
//		if (node->key[nodeNo] == _key) //Ключ найден
//		{
//			break;
//		}
//		if (node->leaf) // Ключ не найден
//		{
//			return 0;
//		}
//		parent = node;
//		parentNo = nodeNo;
//		node = node->child[nodeNo];
//	}
//
//	/* Часть удаления */
//	if (node->leaf) // Из листа
//	{
//		if (node->nkeys > t - 1) //Ключей больше t-1: Просто удаляем
//		{
//			delete node->value[nodeNo];
//			for (int i = nodeNo; i < node->nkeys - 1; i++)
//			{
//				node->key[i] = node->key[i + 1];
//				node->value[i] = node->value[i + 1];
//			}
//			node->nkeys--;
//		}
//		else // Ключей меньше t-1
//		{
//			bool first, last;
//			last = parentNo == parent->nkeys;
//			first = parentNo == 0;
//			BNode<V>* neighbor = NULL;
//
//			// Есть соседний c nkeys > t-1
//			if (!last)
//			{
//				if (parent->child[parentNo + 1]->nkeys > t - 1) // Сосед справа
//				{
//					/* если существует соседний лист (находящийся рядом с ним
//					и имеющий такого же родителя), который содержит больше t-1
//					ключа, то выберем ключ из этого соседа, который является
//					разделителем между оставшимися ключами узла-соседа и
//					исходного узла (то есть не больше всех из одной группы и
//					не меньше всех из другой). Пусть это ключ k1. Выберем
//					ключ k2 из узла-родителя, который является разделителем
//					исходного узла и его соседа, который мы выбрали ранее.
//					Удалим из исходного узла нужный ключ (который необходимо
//					было удалить), спустим k2 в этот узел, а вместо k2 в
//					узле-родителе поставим k1.
//					*/
//					neighbor = parent->child[parentNo + 1];
//
//					delete node->value[nodeNo];
//					for (int i = nodeNo; i < node->nkeys - 1; i++)
//					{
//						node->key[i] = node->key[i + 1];
//						node->value[i] = node->value[i + 1];
//					}
//					node->key[node->nkeys - 1] = parent->key[parentNo];
//					node->value[node->nkeys - 1] = parent->value[parentNo];
//
//					parent->key[parentNo] = neighbor->key[0];
//					parent->value[parentNo] = neighbor->value[0];
//
//					for (int i = 0; i < neighbor->nkeys - 1; i++)
//					{
//						neighbor->key[i] = neighbor->key[i + 1];
//						neighbor->value[i] = neighbor->value[i + 1];
//					}
//					neighbor->nkeys--;
//					first = 1;
//				}
//			}
//			if (!first)
//			{
//				if (parent->child[parentNo - 1]->nkeys > t - 1) // Сосед слева
//				{
//					neighbor = parent->child[parentNo - 1];
//
//					delete node->value[nodeNo];
//					for (int i = nodeNo; i > 0; i--)
//					{
//						node->key[i] = node->key[i - 1];
//						node->value[i] = node->value[i - 1];
//					}
//					node->key[0] = parent->key[parentNo];
//					node->value[0] = parent->value[parentNo];
//
//					parent->key[parentNo] = neighbor->key[nkeys - 1];
//					parent->value[parentNo] = neighbor->value[nkeys - 1];
//
//					neighbor->nkeys--;
//				}
//			}
//			if (neighbor == NULL) // Все соседи по t-1 ключу
//			{
//				/* Если же все соседи нашего узла имеют по t-1 ключу.
//				То мы объединяем его с каким-либо соседом, удаляем нужный
//				ключ. И тот ключ из узла-родителя, который был разделителем
//				для этих двух «бывших» соседей, переместим в наш
//				новообразовавшийся узел (очевидно, он будет в нем медианой).
//				*/
//
//				if (parentNo == parent->nkeys)
//				{
//					parent->mergeRight(parentNo - 1);
//				}
//				else
//				{
//					parent->mergeRight(parentNo);
//				}
//
//				delete newNode->value[nodeNo];
//				for (int i = nodeNo; i < node->nkeys - 1; i++)
//				{
//					node->key[i] = node->key[i + 1];						
//					node->value[i] = node->value[i + 1];
//				}
//				node->nkeys--;
//			}
//		}
//	}
//	else // Не лист 
//	{///TODO
//		if () // Если дочерний узел, предшествующий ключу k содержит больше t-1 ключа
//		{
//			/* то находим k1 – предшественника k в поддереве этого узла. 
//			Удаляем его (рекурсивно запускаем наш алгоритм). Заменяем k 
//			в исходном узле на k1. 
//			
//			Проделываем аналогичную работу, если дочерний узел, следующий за ключом k,
//			имеет больше t-1 ключа.
//			*/
//		}
//		else if () //Если оба (следующий и предшествующий дочерние узлы) имеют по t-1 ключу,
//		{
//			/*то объединяем этих детей, переносим в них k, а далее удаляем k из нового
//			узла(рекурсивно запускаем наш алгоритм).*/
//		}
//		else if () //Если сливаются 2 последних потомка корня
//		{
//			/*то они становятся корнем,	а предыдущий корень освобождается.
//			*/
//		}
//
//	}
//
//
//	return 1;
//}
