# Lecture 10: Nov 7 2019

## Generic lambdas

[`lambda.cpp`](lambda.cpp)
```c++
#include <iostream>

using namespace std;

int main()
{
    auto f = [](auto a, auto b)
    {
        return a + b;
    };

    cout << f(1, 2) << "\n";
    cout << f("Hello "s, "world") << "\n";
}
```

- You can write generic lambdas using `auto` for its arguments.
  - This isn't allowed for regular functions.

## `<algorithm>`

- The standard library provides a long list of algorithms in `<algorithm>`. Most are function templates that take iterator arguments.

[`for_each.cpp`](for_each.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};

    for_each(v.begin(), v.end(), [](auto & i)
                                 {
                                     i++;
                                 });

    for (auto & i : v)
        cout << i << " ";
}
```

- `for_each(first, last, f)` calls a function `f` on each element of the range defined by `first` and `last`.
- A range-based for loop is often simpler, but `for_each` allows the range to be specified.

[`find.cpp`](find.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};

    if (auto result = find(v.begin(), v.end(), 4); result != v.end())
    {
        cout << "4 found\n";
    }
    if (auto result = find(v.begin(), v.end(), 3); result != v.end())
    {
        cout << "3 found\n";
    }

    if (auto result = find_if(v.begin(), v.end(), [](const auto & i)
                                                  {
                                                      return i > 1;
                                                  }); result != v.end())
    {
        cout << "something >1 found: " << *result << "\n";
    }
}
```

- `find` returns an iterator to the first place where a value is found, or the end iterator if no match is found.
- There are also `find_if` and `find_if_not` variants that take a predicate instead.

[`algorithms.cpp`](algorithms.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

int main()
{
    vector<int> v (100);

    iota(v.begin(), v.end(), 0);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    random_device rd;
    default_random_engine g(rd());

    shuffle(v.begin(), v.end(), g);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    replace_if(v.begin(), v.end(), [](auto & i)
                                   {
                                       return i > 50;
                                   }, 50);

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";

    sort(v.begin(), v.end());

    for (auto & i : v)
        cout << i << " ";
    cout << "\n\n";
}
```

- `iota` fills a range with an incrementing sequence.
- The standard library also provide classes (in `<random>`) that generate _random_ and pseudorandom numbers in much more sophisticated ways than C `rand` and `srand`. See the [documentation](https://en.cppreference.com/w/cpp/numeric/random) for more details.
- `shuffle` reorders the range such that any permutation has equal probability to be chosen. It requires a source of random numbers.
- `replace` and `replace_if` replace some elements of a range.
- `sort` sorts a range defined by _random access_ iterators.
  - Other containers like `list` have a member `sort` function, since the implementation has to differ.

### Modifying containers

- None of the algorithms add or remove elements of a container, they only operate on _ranges_ defined by iterators, so they are unable to affect the size of the actual container.

[`remove1.cpp`](remove1.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};
    auto end = remove(v.begin(), v.end(), 1);

    for_each(v.begin(), end, [](auto & i)
                             {
                                 cout << i << " ";
                             });

    cout << "\n";
    for (auto & i : v)
        cout << i << " ";
}
```

- e.g. `remove` shifts the elements in the range to remove some elements, and returns a new end of range iterator. The underlying container's size is unchanged.

[`copy1.cpp`](copy1.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2;

    copy(v1.begin(), v1.end(), v2.begin());
}
```

- e.g. `copy` assumes there is enough space in the destination container to copy the source range.

[`remove2.cpp`](remove2.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};
    auto end = remove(v.begin(), v.end(), 1);

    v.erase(end, v.end());

    for (auto & i : v)
        cout << i << " ";
}
```

- One option to modify a container is by directly referring to it.
- e.g. after calling `remove` call `erase` on the container with the iterator returned by `remove`.

[`copy2.cpp`](copy2.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2 (v1.size());

    copy(v1.begin(), v1.end(), v2.begin());

    for (auto & i : v2)
        cout << i << " ";
}
```

- e.g. Resize the container to the appropriate size before calling copy.

[`copy3.cpp`](copy3.cpp)
```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3};
    vector<int> v2;

    copy(v1.begin(), v1.end(), back_inserter(v2));

    for (auto & i : v2)
        cout << i << " ";
}
```

- You can also use something that knows about the container to modify it.
  - e.g. `back_inserter`, `inserter`, and `front_inserter` (in `<iterator>`) create special iterators that call `push_back`, `insert`, and `push_front` respectively, so the container will grow when things are written to the iterator.

### Parallel algorithms

[`execution.cpp`](execution.cpp)
```c++
#include <execution>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    for (int i = 0; i < 1e2; ++i)
    {
        v.emplace_back(i);
    }

    auto f = [](auto & i)
             {
                 cerr << i << " ";
             };

    for_each(v.begin(), v.end(), f);

    cin.get();
    cerr << "\n";


    for_each(execution::par, v.begin(), v.end(), f);

    cin.get();
    cerr << "\n";

    for_each(execution::par_unseq, v.begin(), v.end(), f);

    // parallelization can help a lot for sort (increase the vector size)
    // sort(execution::par, v.begin(), v.end());
}
```

- Most functions also accept an _execution policy_ (as the first argument), which allows for parallelization.
- If an execution policy is not specified, `execution::seq` (sequential) is the default.
- `execution::par`
  - Permits the algorithm to be parallelized.
- `execution::par_unseq`
  - Permits the algorithm to be parallelized _and_ vectorized (unsequenced execution on a single thread, using [SIMD](https://en.wikipedia.org/wiki/SIMD) instructions.
  - Can break some synchronization methods (more on this next lecture).
- These execution policies are _guidelines_ to the compiler. It is not guaranteed that using them will actually generate parallel code.
- Execution policies are badly supported by `g++`. It was implemented in `g++ 9.1` (earlier this year) using the Intel TBB library (`-ltbb` flag at the end of the command to link the library).
  - This code is unlikely to run on your machine without installing anything new.
  - Use the instructions at [this link](https://solarianprogrammer.com/2019/05/09/cpp-17-stl-parallel-algorithms-gcc-intel-tbb-linux-macos/) to install the dependencies.

## Functional programming in C++

- C++ is a multi-paradigm language.
- We've seen OOP, generic programming, procedural programming so far. Functional programming is also possible.

[`oop.cpp`](oop.cpp)
```c++
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

class test
{
public:
    void print()
    {
        cout << "print()\n";
    }
};

int main()
{
    vector<test> v (10);

    // We'd like to do something like this
    // for_each(v.begin(), v.end(), test::print);

    for_each(v.begin(), v.end(), [](auto & t)
                                 {
                                     t.print();
                                 });

    for_each(v.begin(), v.end(), mem_fn(&test::print));
}
```

- But calling member functions and regular functions are different.
  - One solution is to use a lambda to perform the call.
  - You can also use `mem_fn` (in `<functional>`), which converts a member function to a regular function object.

[`bind.cpp`](bind.cpp)
```c++
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 4};

    using namespace placeholders;

    replace_if(v.begin(), v.end(), bind(less<int>(), _1, 3), 3);

    for (auto & i : v)
        cout << i << " ";
}
```

- `<functional>` provides many functor versions of common operations, like `plus` which just wraps `+`.
- `bind` partially applies a function.
  - In order to leave arguments unbound, use `placeholders::_1`, `placeholders::_2`, etc.

[`higher.cpp`](higher.cpp)
```c++
#include <iostream>
#include <vector>
#include <functional>
#include <numeric>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3, 4};
    vector<int> v2;

    using namespace placeholders;

    transform(v1.begin(), v1.end(), back_inserter(v2), bind(plus<int>(), 1, _1));

    for (auto & i : v2)
        cout << i << " ";

    cout << "\n\n";
    cout << accumulate(v1.begin(), v1.end(), 0, plus<int>());
}
```

- Other common higher-order functions are available as well
  - e.g. `accumulate` (in `<numeric>`) is a fold, `transform` is a map

### Function types

[`recursion.cpp`](recursion.cpp)
```c++
#include <iostream>
#include <functional>

using namespace std;


int main()
{
    function<int(int)> fib = [&fib](int i)
               {
                   if (i <= 1)
                       return 1;
                   return fib(i - 1) + fib(i - 2);
               };

    cout << fib(10);
}
```

- So far we have used templates to take functors/lambdas/function pointers parameters.
- In fact, in the case of lambdas, they have some _unspecified_ unique type, so you cannot write its type without `auto`.
- `function` is a function object that holds any callable function.
- Needed (actually, there are some workarounds without using `function`) for recursive lambdas since `auto` needs to know its own type, but its own type (from `auto`) depends on itself.
  - We need to capture by reference since capturing by value gets an uninitialized `function`.
  - Note that you can't return this lambda since depends on a local variable (itself).

[`functions.cpp`](functions.cpp)
```c++
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class functor
{
public:
    bool operator()(int i)
    {
        return i == 0;
    }
};

bool f2(int i)
{
    return i == 1;
}

int main()
{
    functor f1;

    vector<function<bool(int)>> funcs {f1, f2, [](int i){ return i == 2; }};

    for (auto & f : funcs)
        cout << f(1) << " ";
}
```

- Also useful if you have a heterogeneous collection of functions to call with the same signature, but not necessarily the same type.
- You should prefer templates to `function` if possible though, since `function` can have some runtime overhead. Conceptually, templates are also a "safer" option since they involve static (compile-time) checks.
