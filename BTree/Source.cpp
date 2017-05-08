#include <iostream>
#include <string>
#include <fstream>
#include "BTree.h"
#include <time.h>
class Student;
using namespace std;

#define FILENAME "names.txt"
#define HASH_TABLE_SIZE 1500
const int BTree<Student>::t = 4;

class Student
{
public:
	string name;
	int age;
	unsigned long long phone;
	Student() {};
	Student(string _name, int _age, unsigned long long _phone) : name(_name), age(_age), phone(_phone) {};
	friend ostream& operator<<(ostream& os, const Student& st);
};

ostream& operator<<(ostream& os, const Student& student)
{
	os << "Name: " << student.name << ", Age: " << student.age << ", Phone: +" << student.phone;
	return os;
}

void printName(Student& student)
{
	cout << student.name << endl;
}

void main()
{
	std::ifstream in(FILENAME);
	srand(time(NULL));
	BTree<Student> phoneBook;
	string name;

	// Заполнение дерева
	while (getline(in, name)) 
	{
		Student* cur = new Student(name, 17 + rand() % 10, rand()* rand());
		int hash = std::hash<std::string>()(name) % HASH_TABLE_SIZE;
		while (phoneBook.search(hash)){
			hash = hash + 1 % HASH_TABLE_SIZE;
		}
		phoneBook.insert(hash, cur);
	}

	//Вывод дерева
	phoneBook.print();


	//Поиск в дереве
	cout << "Enter name for search :";
	while (getline(std::cin, name))
	{
		int hash = std::hash<std::string>()(name) % 1500;
		Student value;
		int i = 0; // Счетчик попыток поиска
		do
		{
			phoneBook.search(hash, &value);
			hash = hash + 1 % HASH_TABLE_SIZE;
			i++;
		} while (value.name != name && i < 5);
		if (i < 5)
		{
			cout << value << endl;
		}
		else
		{
			cout << "Not found" << endl;
		}
		cout << "Enter name for search :";
	}

	//Удаление дерева
	//for (int i = 0; i < HASH_TABLE_SIZE; i++)
	//{
	//	if(phoneBook.remove(i))
	//	{
	//		cout << "deleted " << i << endl;
	//	}
	//}
	//phoneBook.print();

	// Обход дерева с выполнением функции
	//phoneBook.traverse(printName);

}