#include <thread>
#include <iostream>

using namespace std;

int f(int i, int & result)
{
    cerr << i << i + 1;

    // not usable in main
    return i + 1;
}

int main()
{
    int i1;
    int i2;

    thread t1 {f, 1, ref(i1)};
    thread t2 {f, 3, ref(i2)};

    t1.join();
    t2.join();
}
