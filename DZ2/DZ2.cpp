#include <algorithm>
#include <cassert>
#include <vector>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "Timer.h"
using namespace std;

//---------------------------------1------------------------------

template<typename T>
void Swap(T*& firstPtr, T*& secondPtr)
{
	T* tempPtr = move(firstPtr);
	firstPtr = move(secondPtr);
	secondPtr = move(tempPtr);
}

void Task1() 
{
	int a = 1;
	int b = 2;
	int* ptra = &a;
	int* ptrb = &b;
	cout << "a  " << ptra << setw(3) << a << endl;
	cout << "b  " << ptrb << setw(3) << b << endl;
	Swap(ptra, ptrb);
	cout << "a  " << ptra << setw(3) << a << endl;
	cout << "b  " << ptrb << setw(3) << b << endl;
	cout << endl;
}
//---------------------------------2------------------------------

template<typename T>
void SortPointers(vector<T*>& p)
{
    sort(p.begin(), p.end(), [](const auto& p1, const auto& p2)
        {
            return *p1 < *p2;
        });
}

void Task2() 
{
	srand(time(0));
	int length = 7;
	vector<int*> p;
	for (int i = 0; i < length; ++i)
	{
		int* a = new int;
		*a = rand() % 10;
		p.push_back(a);
	}
	for (const auto& i : p)
	cout << *i << " ";
	cout << endl;
	SortPointers(p);
	for (const auto& i : p)
	cout << *i << " ";
	cout << endl;
	cout << endl;
}

//---------------------------------3------------------------------

class Counter
{
public:
	Counter(ifstream& file);
	int CountCount_IfFind();
	int CountCount_IfFor();
	int CountForFind();
	int CountTwoFor();
	static string vowels_;
	string text_;
};

Counter::Counter(ifstream& file)
{
	if (file.is_open())
	{
		string tmp;
		while (!file.eof())
		{
			getline(file, tmp, '\n');
			text_.append(tmp);
		}
	}
}

int Counter::CountCount_IfFind()
{
	Timer timer("Count_If + Find");

		int result = count_if(Counter::text_.begin(), Counter::text_.end(),
		[](const char& value)
		{
			return (Counter::vowels_.find(value) != Counter::vowels_.npos);
		});

	timer.print();
	return result;
}

int Counter::CountCount_IfFor()
{
	Timer timer("Count_If + For");

	int result = count_if(Counter::text_.begin(), Counter::text_.end(),
	[](const char& value)
	{
		for (const char& it : Counter::vowels_)
		{
			if (it == value)
			return true;
		}
			return false;
		});

	timer.print();
	return result;
}

int Counter::CountForFind()
{
	Timer timer("For + Find");

	int result = 0;

	for (const char& ch : Counter::text_)
	{
		if (Counter::vowels_.find(ch) != Counter::vowels_.npos)
			++result;
	}
	timer.print();
	return result;
}

int Counter::CountTwoFor()
{
	Timer timer("For + For");

	int result = 0;

	for (const char& ch : Counter::text_)
	{
		for (const char& vow : Counter::vowels_)
			if (ch == vow)
				++result;
	}
	timer.print();
	return result;
}

string Counter::vowels_ = { 'A', 'E', 'I', 'O', 'U', 'Y','a', 'e', 'i', 'o', 'u', 'y'};

void Task3() 
{
	ifstream file("d:\\file W&P.txt");
	if (!file)
	{
		cout << "File is not open!\n\n";
	}
	else cout << "All OK! File open!\n\n";
	
	Counter counter(file);

	cout << counter.CountCount_IfFind() << endl;		//  117.433 ms
	cout << counter.CountCount_IfFor() << endl;		//  175.12 ms
	cout << counter.CountForFind() << endl;			//  108.233 ms
	cout << counter.CountTwoFor() << endl;			    //  160.498 ms
}

int main()
{
	Task1();
	Task2();
	Task3();
}

