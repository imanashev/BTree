#include <iostream>
#include <string>
#include <fstream>
#include "BTree.h"
#include <time.h>
#define FILENAME "names.txt"

using namespace std;

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

ostream& operator<<(ostream& os, const Student& st)
{
	os << "Name: " << st.name << ", Age: " << st.age << ", Phone: +" << st.phone;
	return os;
}

void main()
{
	std::ifstream in(FILENAME);
	srand(time(NULL));
	BTree<Student> tree;
	string name;

	while (getline(in, name))
	{
		Student* cur = new Student(name, 18 + rand() % 10, rand()* rand());
		int hash = std::hash<std::string>()(name) % 1500;
		tree.insert(hash, *cur);
	}
	tree.traverse();

	cout << "Enter name for search :";
	while (getline(std::cin, name))
	{
		int hash = std::hash<std::string>()(name) % 1500;
		Student value;
		tree.search(hash, &value);
		cout << value << endl;
		cout << "Enter name for search :";
	}
	/*
	for (int i = 0; i < 50; i++)
	{
	int j = rand() % 1000;
	a = "stringa_" + to_string(j);
	tree.insert(j, a);
	}
	*/
}