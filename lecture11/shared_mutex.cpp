#include <thread>
#include <iostream>
#include <shared_mutex>
#include <utility>

using namespace std;

shared_mutex sm;

pair<int, int> state;

void write()
{
    for (int i = 0; ; ++i)
    {
        this_thread::sleep_for(1s);

        unique_lock lock {sm};
        state.first = i;
        state.second = i + 1;
    }
}

void read()
{
    while (true)
    {
        this_thread::sleep_for(1s);

        shared_lock lock {sm};
        cerr << this_thread::get_id() << ": " << state.first << "," << state.second << "\n";
    }
}

int main()
{
    thread w1 {write};
    thread w2 {write};

    thread r1 {read};
    thread r2 {read};
    thread r3 {read};

    w1.join();
    w2.join();
    r1.join();
    r2.join();
    r3.join();
}
