

#include <iostream>
#include <iomanip>
#include <optional>
#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std;
namespace MyNamespace
{
	class FullName
	{
	private:
		string surname, name;
		optional<string> patronymic;

	public:
		FullName(string surname, string name, string patronymic) : surname(surname), name(name), patronymic(patronymic) {}
		FullName(string surname, string name) : surname(surname), name(name), patronymic(nullopt) {}
		~FullName()
		{
			this->surname = "";
			this->surname.shrink_to_fit();
			this->name = "";
			this->name.shrink_to_fit();
			this->patronymic.reset();
		}
		bool operator==(const FullName& name)
		{
			return tie(this->surname, this->name, this->patronymic) == tie(name.surname, name.name, name.patronymic);
		}
		friend ostream& operator<<(ostream& os, const FullName fn);
	};
	ostream& operator<<(ostream& os, const FullName fn)
	{
		os << "Полное имя: \t" << fn.surname << ' ' << fn.name;
		if (fn.patronymic.has_value())
		{
			os << ' ' << fn.patronymic.value();
		}
		return os;
	}
	class Student
	{
	private:
		FullName fullName;
		vector<int> scores;
		double averangeScore = 0;

	public:
		Student(string surname, string name, string patronymic, vector<int> scores) : fullName(surname, name, patronymic), scores(scores)
		{
			for (auto iter = 0; iter < scores.size(); iter++)
			{
				this->averangeScore += scores.at(iter);
			}
			this->averangeScore /= scores.size();
		}
		Student(string surname, string name, vector<int> scores) : fullName(surname, name)
		{
			for (auto iter = 0; iter < scores.size(); iter++)
			{
				this->scores.push_back(scores[iter]);
			}
			for (auto iter = 0; iter < scores.size(); iter++)
			{
				this->averangeScore += scores.at(iter);
			}
			this->averangeScore /= scores.size();
		}
		~Student()
		{
			if (!scores.empty())
			{
				this->scores.clear();
				this->scores.shrink_to_fit();
			}
		}
		double GetAverageScore()
		{
			return this->averangeScore;
		}
		bool operator==(const FullName& name)
		{
			return this->fullName == name;
		}
		friend ostream& operator<<(ostream& os, const Student st);
	};
	ostream& operator<<(ostream& os, const Student st)
	{
		os << st.fullName << endl
			<< "Средний балл: \t" << st.averangeScore << endl << "Все баллы:\n";
		for (auto iter = 0; iter < st.scores.size(); iter++)
		{
			os << st.scores[iter] << '\t';
			if (iter % 5 == 4)
			{
				os << endl;
			}
		}
		os << endl;
		return os;
	}

	class IRepository
	{
		virtual void Open() = 0; // бинарная десериализация в файл
		virtual void Save() = 0; // бинарная сериализация в файл
	};

	class IMethods
	{
		virtual double GetAverageScore(const FullName& name) = 0;
		virtual string GetAllInfo(const FullName& name) = 0;
		virtual string GetAllInfo() = 0;
	};

	class StudentsGroup : public IRepository, public IMethods
	{
	private:
		vector<Student> students;

	public:
		StudentsGroup(string surname, string name, string patronymic, vector<int> scores)
		{
			students.push_back(Student(surname, name, patronymic, scores));
		}
		StudentsGroup(string surname, string name, vector<int> scores)
		{
			students.push_back(Student(surname, name, scores));
		}
		StudentsGroup() {}
		void addStudent(string surname, string name, string patronymic, vector<int> scores)
		{
			students.push_back(Student(surname, name, patronymic, scores));
		}
		void addStudent(string surname, string name, vector<int> scores)
		{
			students.push_back(Student(surname, name, scores));
		}
		void Open() override
		{
			ifstream in("binary.bin", ios::binary);
			in.read(reinterpret_cast<char*>(&this->students), sizeof(this->students));
		}
		void Save() override
		{
			ofstream out("binary.bin", ios::binary);
			if (out.good())
			{
				out.write(reinterpret_cast<char*>(&this->students), sizeof(this->students));
			}
		}
		double GetAverageScore(const FullName& name)
		{
			return (*find_if(this->students.begin(), this->students.end(), [&name](auto iter)
				{ return iter == name; })).GetAverageScore();
		}
		string GetAllInfo(const FullName& name)
		{
			stringstream ss;
			ss << (*find_if(this->students.begin(), this->students.end(), [&name](auto iter)
				{ return iter == name; }));
			return ss.str();
		}
		string GetAllInfo()
		{
			stringstream ss;
			for (auto iter : this->students)
			{
				ss << iter << endl;
			}
			return ss.str();
		}

		~StudentsGroup()
		{
			if (!students.empty())
			{
				students.clear();
				students.shrink_to_fit();
			}
		}
	};

}

int main()
{
	setlocale(LC_ALL, "Russian");

	vector<int> aver1{ 50,100,88,75,64,92 };
	vector<int> aver2{ 84,16,85,37,61,52,65,95,13,48,6,26,9 };
	MyNamespace::StudentsGroup group1("Иванов", "Иван", "Иванович", aver2);
	group1.addStudent("Петров", "Петр", aver2);
	cout << group1.GetAllInfo();
	MyNamespace::StudentsGroup group2;
	cout << group2.GetAllInfo();
	group1.Save();
	group2.Open();
	cout << group2.GetAllInfo();
	cout << group2.GetAllInfo(MyNamespace::FullName("Петров", "Петр"));


	return 0;
}

