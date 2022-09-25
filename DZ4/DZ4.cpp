#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <algorithm>
#include <random>
#include <set>
using namespace std;

void Insert_Sorted(vector<int>& v, int num)
{
	auto it = upper_bound(v.begin(), v.end(), num);
	v.insert(it, num);
}

template<class T1, class T2>
void Insert_Sorted(T1& v, T2 num)
{
	auto it = upper_bound(v.begin(), v.end(), num);
	v.insert(it, num);
}

template<class T1>
void PrintContainer(T1& v)
{
	for_each(v.begin(), v.end(), [](auto i) { cout << i << " "; });
	cout << endl;
}

int main()
{
	cout << "Task 1" << endl << endl;

	vector<int> v1{ 1, 2, 3, 5, 6, 7 };
	Insert_Sorted(v1, 4);
	PrintContainer(v1);
	vector<string> v2{ "a", "b", "d", "e" };
	Insert_Sorted(v2, "c");
	PrintContainer(v2);
	set<int> s{ 1,3,1,1 };
	Insert_Sorted(s, 2);
	PrintContainer(s);
	cout << endl;

	cout << "Task 2" << endl << endl;


	vector<double> analogSignal(20);
	srand(time(0));
	generate(analogSignal.begin(), analogSignal.end(), []() { return (static_cast<double>(rand() % 100) / ((rand() % 10) + 1)); });
	PrintContainer(analogSignal);
	vector<int> digitalSignal(20);
	copy(analogSignal.begin(), analogSignal.end(), digitalSignal.begin());
	PrintContainer(digitalSignal);
	double error = 0;
	for_each(analogSignal.begin(), analogSignal.end(), [&](const double& i){error += pow(i - static_cast<int>(i), 2);});
	cout << "Error = " << error << endl;
}

