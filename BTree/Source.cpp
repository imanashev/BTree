#include <iostream>
#include <string>
#include <fstream>
#include "BTree.h"
#include <time.h>

#define FILENAME "names.txt"
#define HASH_TABLE_SIZE 1500


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
	BTree<Student> phoneBook;
	string name;

	while (getline(in, name)) // Заполнение дерева
	{
		Student* cur = new Student(name, 17 + rand() % 10, rand()* rand());
		int hash = std::hash<std::string>()(name) % HASH_TABLE_SIZE;
		while (phoneBook.search(hash)){
			hash = hash + 1 % HASH_TABLE_SIZE;
		}
		phoneBook.insert(hash, cur);
	}
	phoneBook.traverse();


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
}