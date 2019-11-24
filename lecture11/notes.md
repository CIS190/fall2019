# Lecture 11: Nov 21 2019

## Concurrency

- Useful for:
  - Improving throughput
  - Improving responsiveness
- We saw limited support for this in the form of parallel algorithms from last class.
- C++ also supports other types of concurrency facilities in its standard library.

- We will call a _task_ a computation that can be executed concurrently with other tasks. A task will be a function or function object.
- A thread is the representation of a task by the OS. Threads can be run concurrently, either in parallel (on different CPU cores) or by context switching (running parts of a thread before switching to another thread)

- To compile the code in this lecture you will need to link against the `pthread` library using `-lpthread` at the end of your `g++` command.
  - This should only work on *nix environments. If you are using Visual C++ on Windows, it should work without having to do anything extra.

### Issues with concurrency

- Data races
- Deadlock

### `thread`s

[`thread.cpp`](thread.cpp)
```c++
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
```

- A `thread` (in `<thread>`) is what the standard library provides to represent a thread.
  - Pass the function (or functor) and its arguments separately to `thread`.
- As soon as a `thread` is initialized with a task, it starts running.
- `.join()` on a `thread` blocks the current `thread` until that `thread` finishes its task.
- `.detach()` on a `thread` will allow the `thread` to run independently, without having to be joined.
  - This is useful because the destructor of `thread` will terminate the program if it is called without `.join()` or `.detach()` being called (so signalling your intent is required).
- The return value of a task in a `thread` is ignored.
- We can use a reference to return a value from a `thread`.
  - However, the way we pass arguments to the `thread` constructor uses a variadic template that can accept any number of arguments. This doesn't work well with references for technical reasons so we have to wrap the variables with `ref` (from `<functional>`) to force the compiler to see that these arguments are references.
    - `bind` from last lecture has the same problem with references.

## "Higher-level" concurrency

### `async`

[`async.cpp`](async.cpp)
```c++
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int f(int i)
{
    cout << "f called\n";

    this_thread::sleep_for(1s);

    return i + 1;
}

int main()
{
    // By default async is allowed to do either async or deferred
    auto result = async(f, 12);

    // You can also specify explicitly
    // auto result = async(launch::async, f, 12);
    // auto result = async(launch::deferred, f, 12);

    this_thread::sleep_for(1s);

    cout << "waiting for result\n";

    cout << result.get();
}
```

- `async` (in `<future>`) allows you to perform a task _asynchronously_, that is, without waiting for it to finish, and get its return value later.
  - `async` separates the call part of a function call from its return value.
- The return value is a `future`. Use `.get()` on the `future` to get the result of the function call.
  - If the result is not available yet, `.get()` will block until it is.
  - `.get()` can only be used once. Multiple uses is undefined (use `.valid()` to check if it is available to use).
- `async` _may_ run a task in a separate thread, or just run it synchronously when its result is queried using `.get()`. This is decided at runtime.
  - You can explicitly choose by passing in `std::launch::async` or `std::launch::deferred` as the first argument.
- Only use `async` for "simple" tasks (e.g. don't edit any shared variables, don't require explicit synchronization).
  - Some constructs, like `mutex` (more on this later), have behavior that depends on the current `thread`. Since `async` may or may not choose to run your task in a new thread, this can cause some nasty bugs.

[`async_exception.cpp`](async_exception.cpp)
```c++
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int f(int i)
{
    throw 1;
}

int main()
{
    auto result = async(f, 12);

    try
    {
        cout << result.get();
    }
    catch (int i)
    {
        cout << "Caught " << i;
    }
}
```

- `async` will also propagate any exceptions thrown in the task.

[`async_sync.cpp`](async_sync.cpp)
```c++
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int f(int i)
{
    cout << "f called\n";

    this_thread::sleep_for(1s);

    return i + 1;
}

int main()
{
    async(f, 12);
    async(f, 12);
}
```

- The destructor of the returned `future` will block until the asynchronous call is complete.
  - This means if you don't bind the returned `future` to a variable, it will be immediately destructed and the `async` call will be essentially synchronous.
  - Note that this behavior of `~future` is only for `future`s obtained from `async`.

### `future`s and `promise`s

[`promise.cpp`](promise.cpp)
```c++
#include <future>
#include <thread>
#include <iostream>

using namespace std;

void producer(int i, promise<int> & p)
{
    p.set_value(i + 1);
}

int main()
{
    promise<int> p;

    future<int> result {p.get_future()};

    thread t {producer, 12, ref(p)};

    cout << result.get();

    t.join();
}
```

- Above, `async` returned a `future`, which represents the result of an asynchronous operation.
  - This asynchronous operation was a function call, and the result was the return value.
- More generally, a `future` can represent any result, not just the results of functions.
- A `promise` allows a task to pass a result to its corresponding `future`.
  - Use `set_value` to return a normal result.
  - Use `set_exception` to throw an exception when the associated `future` has `.get()` called.
- Use `.get_future()` to get the `future` associated with a `promise`. This function can only be called once.

[`shared_future.cpp`](shared_future.cpp)
```c++
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
```

- Sometimes many threads will want the result of a `future`, but `.get()` can only be called once. We could take the return value of `.get()`, but then we have to wait until it's ready.
- `shared_future` allows multiple `thread`s to wait on its result.
- A `future` can be implictly converted to a `shared_future` (by one of `shared_future`'s constructors), or created by `future::share()`, which transfers the state of the `future` to the returned `shared_future` (so the original `future` is then invalid).

### `packaged_task`

[`packaged_task.cpp`](packaged_task.cpp)
```c++
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
```

- `packaged_task` wraps a task with a `promise`, allowing us to easily use the return value of, or any exceptions throw from a task.
- Since a `package_task` owns resources (the task and `promise`), it cannot be copied and must be `move`d.
- Using `packaged_task`s and `thread`s is similar to, but more flexible than `async`, since we can control how the task is run (whereas `async` decides for us if it should run in a separate thread, and starts the task immediately if it does).

## "Lower-level" concurrency

### `mutex`

[`mutex.cpp`](mutex.cpp)
```c++
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
```

- A `mutex` (also commonly called a lock) (in `<mutex>`) is a synchronization mechanism.
  - A `thread` can `.lock()` a `mutex` to _own_ the `mutex`.
  - If another `thread` owns the mutex, the call to `.lock()` blocks until the `mutex` is no longer owned.
  - To release ownership, the owning `thread` must call `.unlock()`.
- The code between a `.lock()` and `.unlock()` is called a critical section. Multiple `threads` cannot access it concurrently.

### Locking `mutex`es

[`mutex_exception.cpp`](mutex_exception.cpp)
```c++
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void f(int i, int & result)
{
    try
    {
        m.lock();
        cerr << i << i + 1;
        throw 1;
        m.unlock();
    }
    catch (...)
    {}
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
```

- Using `.lock()` and `.unlock()` is error-prone and breaks exception safety, just like using `new`/`delete`.

[`scoped_lock.cpp`](scoped_lock.cpp)
```c++
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void f(int i, int & result)
{
    try
    {
        scoped_lock lock {m};

        cerr << i << i + 1;
        throw 1;
    }
    catch (...)
    {}
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
```

- A `mutex` is a resource as well, so RAII is a useful tool here.
- `scoped_lock` is a wrapper around a `mutex` that uses RAII to unlock its `mutex` when it is destroyed.

[`deadlock.cpp`](deadlock.cpp)
```c++
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m1; // protects i1
mutex m2; // protects i2

int i1;
int i2;

void f(int i)
{
    // The loops are to make the deadlock more common, since it's rare that the two mutex locks happen in the right sequence to trigger the deadlock.
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l1 {m1};
        scoped_lock l2 {m2};

        i1 = i;
        i2 = i;

        cerr << "f";
    }
}

void g(int i)
{
    for (int j = 0; j < 100; ++j)
    {
        scoped_lock l2 {m2};
        scoped_lock l1 {m1};

        i1 = i;
        i2 = i;

        cerr << "g";
    }
}

int main()
{
    thread t1 {f, 1};
    thread t2 {g, 3};

    t1.join();
    t2.join();
}
```

- Deadlocks occur when `thread`s acquire and hold a `mutex` while waiting for another `mutex`. If multiple threads do this and are waiting for each other to release a `mutex`, this results in a deadlock.

[`scoped_lock_deadlock.cpp`](scoped_lock_deadlock.cpp)
```c++
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m1; // protects i1
mutex m2; // protects i2

int i1;
int i2;

void f(int i)
{
    for (int j = 0; j < 10000; ++j)
    {
        scoped_lock l {m1, m2};

        i1 = i;
        i2 = i;

        cerr << "f";
    }
}

void g(int i)
{
    for (int j = 0; j < 10000; ++j)
    {
        scoped_lock l {m2, m1};

        i1 = i;
        i2 = i;

        cerr << "g";
    }
}

int main()
{
    thread t1 {f, 1};
    thread t2 {g, 3};

    t1.join();
    t2.join();
}
```

- `scoped_lock` can also wrap multiple `mutex`es, and uses a deadlock-avoiding algorithm to acquire the locks.
  - `scoped_lock` will never block while holding the lock for one of its `mutex`es.

### Sharing with reader and writer threads

[`shared_mutex.cpp`](shared_mutex.cpp)
```c++
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
```

- One common pattern is that there are some `thread`s that create data, and other `thread`s that just read the data. While there would a data race if one `thread` accesses the data while some other `thread` is writing to it, it is fine for multiple `thread`s to read the data at once.
- `shared_mutex` is a mutex for this situation. It has two levels of locking, a _shared_ level and an _exclusive_ level.
  - Multiple `thread`s can lock the `shared_mutex` at the shared level, and only if the exclusive lock is not acquired.
  - Only one `thread` can lock the `shared_mutex` at the exclusive level at a time, and only if the shared lock is not acquired by anyone.
- There are also RAII-style wrappers for shared and exclusive locking, called `shared_lock` and `unique_lock` respectively.
- In this example, multiple reader threads are in the critical section at the same time, which you can see by the interleaved prints.

### Waiting

[`condition_variable.cpp`](condition_variable.cpp)
```c++
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
```

- `mutex` allows us to block until an exclusive resource (the `mutex`) is available. But what if we want to wait for something more complex?
  - We could just use a loop to continually check the condition: `while (condition = false) {} // afterwards condition must be true!`
  - This is both inefficient (wasting CPU time) and doesn't work (the condition may change back to false if we don't guard the condition with a `mutex`, and we never release the `mutex` if we do guard the condition).
- `condition_variable` are a synchronization primitive used for threads to sleep until another thread wakes it up, based on some _condition_.
- `condition_variable` must be used with a `mutex`.
  - The waiting `thread` acquires the `mutex` using a `unique_lock`, then calls `.wait()` on the `condition_variable`. `.wait()` will release the lock and suspend the `thread` until it is woken up.
  - To wake up threads waiting on `condition_variable`s, a `thread` that changes the condition can call `.notify_one()` or `.notify_all()` to wake up one or all waiting `thread`s on that `condition_variable`.
  - When a thread wakes up, it must acquire the lock again to leave the `.wait()`.
- `condition_variable` uses `unique_lock` instead of `scoped_lock` because we need finer control of the lock. `scoped_lock` just locks on construction and unlocks on destruction. `unique_lock` provides methods to lock and unlock at any time.
- `.wait()` can take a second argument, a predicate that represents the condition to check.
  - This is to combat _spurious wakeup_, where suspended threads can just wake up randomly, for no reason.
  - Typically the `wait` is wrapped in a while loop: `while (!condition) cv.wait(...);`.
  - This overload of `.wait()` does this for you, the predicate is checked and if it is not true, we wait again.
    - Note that since `.wait()` needs to acquire the lock, the predicate is guarded by the lock as well.

### C++20

- C++20 will add some other common synchronization primitives: semaphores and barriers.
