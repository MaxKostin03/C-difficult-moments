#include <list>
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <iomanip>
using namespace std;

template<typename T>
void printUnique(T begin, T end)
{
    unordered_set<string> s(begin, end);
    copy(s.begin(), s.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
}


int main()
{
    //-----------------------1-------------------------
    cout << "Task1" << endl;
    vector<string> v{ "one", "two", "three", "four", "four", "one", "five", "six" };
    printUnique(v.begin(), v.end());
    list<string> l(v.begin(), v.end());
    printUnique(l.begin(), l.end());
    deque<string> d(v.begin(), v.end());
    printUnique(d.begin(), d.end());

    //-----------------------2-------------------------
    cout << "Task2" << endl;
    multimap<int, string> str;
    string text;
    const string mark = ".!?";
    cout << "Enter text:  ";
    getline(cin, text);
    text.push_back('.');
    while (!text.empty())
    {
        const size_t pos = text.find_first_of(mark);
        string s_temp = text.substr(0, pos);
        text.erase(0, pos + 2);
        if (s_temp.empty()) { continue; }
        const auto key = count_if(s_temp.begin(), s_temp.end(), [](const char ch) {return isspace(ch); });
        str.emplace(key, s_temp);
    }
    for_each(str.begin(), str.end(), [](const pair<int, string>& str)
        { cout << str.second << "    word count " << str.first + 1 << endl; });
    return 0;
}

