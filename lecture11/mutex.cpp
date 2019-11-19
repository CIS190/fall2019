#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void f(int i, int & result)
{
    m.lock();
    cerr << i << i + 1;
    m.unlock();
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
