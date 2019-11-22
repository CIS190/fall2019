#include <future>
#include <thread>
#include <iostream>

using namespace std;

int f(int i)
{
    return i + 1;
}

int main()
{
    packaged_task<int(int)> pt {f};
    future<int> result {pt.get_future()};

    thread t {move(pt), 12};

    cout << result.get();

    t.join();
}
