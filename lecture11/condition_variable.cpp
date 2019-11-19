#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <chrono>

using namespace std;

queue<int> q;
condition_variable cond;
mutex m;

void consume()
{
    while (true)
    {
        unique_lock lock {m};
        cond.wait(lock, [](){ return !q.empty(); });

        cerr << this_thread::get_id() << ": " << q.front() << "\n";
        q.pop();
    }
}

int main()
{
    thread c1 {consume};

    // We can add more consumers
    // thread c2 {consume};
    // thread c3 {consume};

    for (int i = 0; ; ++i)
    {
        {
            scoped_lock lock {m};
            q.push(i);
        }
        this_thread::sleep_for(1s);

        // What if we change to notify_all with multiple consumers?
        cond.notify_one();
    }
}
