

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <array>
#include <iomanip>

using namespace std;

//--------------------------------1------------------------------
void addToEndList(list<float>& lst)
{
	auto it = lst.begin();
	float result = 0, counter = 0;
	while (it != lst.end())
	{
		result += *it;
		++counter;
		it = next(it);
	}
	result /= counter;
	lst.push_back(result);
}

void PrintList(list<float> lst)
{
	auto it = lst.begin();
	while (it != lst.end())
	{
		cout << *it << " ";
		it = next(it);
	}
	cout << endl;
}

void Task1()
{
	list<float> lst = { 0.7, 2.0, 2.9, 3, 3.9, 4, 7, 8, 9 };
	PrintList(lst);
	addToEndList(lst);
	PrintList(lst);
	cout << endl;
}

//--------------------------------2------------------------------

class Matrix
{
	size_t m_size;
	vector<vector<int>> matrix;
public:
	Matrix(int _size) : m_size(_size) { create(); };

	int Det() { return findDeterminant(matrix, m_size); };

	int findDeterminant(auto a, int n) {
		if (n == 1)
			return a[0][0];
		else if (n == 2)
			return a[0][0] * a[1][1] - a[0][1] * a[1][0];
		else {
			int d = 0;
			for (int k = 0; k < n; k++) {
				vector<vector<int>> m(n - 1, vector<int>(n - 1));
				for (int i = 1; i < n; i++) {
					int t = 0;
					for (int j = 0; j < n; j++) {
						if (j == k)
							continue;
						m[i - 1][t] = a[i][j];
						t++;
					}
				}
				d += pow(-1, k + 2) * a[0][k] * findDeterminant(m, n - 1);
			}
			return d;
		}
	};

	friend ostream& operator<<(ostream& out, Matrix m) {
		for (auto& it : m.matrix)
		{
			for (auto it2 : it)
				out << setw(3) << it2;
			out << endl;
		}
		return out;
	}
private:
	void create() {
		matrix.resize(m_size);
		for (auto& it : matrix)
		{
			it.resize(m_size);
			for (auto& it2 : it)
			{
				it2 = (rand() % 50);
			}
		}
	}
};

void Task2() 
{
	Matrix mat(3);
	cout << mat << endl;
	cout << "Determinant = " << mat.Det() << endl;
	cout << endl;
}

//--------------------------------2------------------------------

template<typename T>
class MyIt
{
public:
	MyIt() {};
	MyIt(T& val) { ptr = &val; };

	void operator=(T val) { *ptr = val; }
	T operator*() { return *ptr; }
	void operator++() { ptr++; }
	void operator--() { ptr--; }
private:
	T* ptr;
};

void Task3() 
{
	int Arr[5] = { 0, 5, 7, 9, 10 };
	MyIt it = *end(Arr);
	--it;
	cout << *it << " ";
	--it;
	cout << *it << " ";
	++it;
	cout << *it << " ";
	cout << endl;
}

int main()
{
	Task1();
	Task2();
	Task3();
}

