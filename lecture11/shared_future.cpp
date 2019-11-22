#include <future>
#include <thread>
#include <iostream>

using namespace std;

void producer(int i, promise<int> & p)
{
    p.set_value(i + 1);
}

void consumer(shared_future<int> & result)
{
    cout << result.get();
}

int main()
{
    promise<int> p;

    future<int> result {p.get_future()};

    thread t {producer, 12, ref(p)};

    shared_future<int> shared = result.share();

    thread t1 {consumer, ref(shared)};
    thread t2 {consumer, ref(shared)};
    thread t3 {consumer, ref(shared)};

    t.join();

    t1.join();
    t2.join();
    t3.join();
}
