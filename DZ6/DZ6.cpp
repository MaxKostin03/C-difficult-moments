#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <random>
#include <iostream>
#include "Timer.cpp"
using namespace std;

//----------------------------1----------------------------
mutex m1;
void pcout(const int num) {
    lock_guard m1_(m1);
    cout << "This thread is: " << num << endl;
}

//----------------------------2----------------------------
int PrimeNumber(int count)
{
    int result;
    int i = 3;
    while (true)
    {
        bool check = true;
        for (int j = 3; j < static_cast<int>(sqrt(i) + 1); j += 2)
        {
            if (i % j == 0) {
                check = false;
                break;
            }
        }
        if (check)
        {
            --count;
            result = i;
            cout << result << " ";
            if (!(count - 1))
            {
                return result;
            }
        }
        else
        {
            check = true;
        }
        i += 2;
    }
    return {};
}

void ThreadPrimeNumber(int count)
{
    int result{};
    mutex m;
    cout << "Prime number: ";
    thread t1([&]()
        {
            lock_guard<mutex>g(m);
            result = PrimeNumber(count);
        });

    thread t2([&]()
        {
            this_thread::sleep_for(0.5s);
            lock_guard<mutex>g(m);
            cout << result << endl;
        });
    t1.join();
    t2.join();
}

//----------------------------3----------------------------

void Steal(vector<int>owen)
{
    mutex m;
    srand(time(0));
    for (int i = 0; i < 10; ++i)
    {
        int mt = rand() % 100;
        thread t1([&]()
        {
                this_thread::sleep_for(1s);
                int number = mt;
                lock_guard<mutex>g(m);
                owen.push_back(number);
                cout << "All items :";
                for (const auto& n : owen)
                {
                    cout << n << " ";
                }
                cout << endl;
        });

        thread t2([&]()
        {
                this_thread::sleep_for(0.5s);
                lock_guard<std::mutex>g(m);

                auto it = max_element(owen.begin(), owen.end());
                if (it != owen.end())
                {
                    cout << "Stolen item: " << *it << endl;
                    owen.erase(remove(owen.begin(), owen.end(), *it));
                }
        });
        t1.join();
        t2.join();
    }
       
}

//----------------------------3----------------------------

int main()
{
    cout << "TASK1" << endl;

    thread t_1(pcout, 1);
    thread t_2(pcout, 2);
    t_1.join();
    t_2.join();

    cout << "TASK2" << endl;
    int count = 400;
    ThreadPrimeNumber(count);

    cout << "TASK3" << endl;

    srand(time(0));
    int length = 7;
    vector<int> p;
    for (int i = 0; i < length; ++i)
    {
        int a = rand() % 100;
        p.push_back(a);
    }
    cout << "All things before theft: ";
    for (const auto& i : p)
    {
        cout << " " << i << " ";
    }
        cout << endl; 
    Steal(p);
}

