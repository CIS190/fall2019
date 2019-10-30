# Lecture 9: Oct 31 2019

### Type aliases for templates

[`using.cpp`](using.cpp)
```c++
#include <iostream>
#include <vector>
#include <list>

using namespace std;

// in vector/list/etc
// using value_type = T;
// same as typedef T value_type;

template <typename Container>
typename Container::value_type sum(const Container & c)
{
    // need typename to tell C++ this is a type
    typename Container::value_type total {};
    for (auto & i : c)
    {
        total += i;
    }

    return total;
}

int main()
{
    vector<int> v {1, 2, 3};
    list<string> l {"h", "e", "l", "l", "o"};

    cout << sum(v) << "\n";
    cout << sum(l) << "\n";
}
```

- All containers provide a few common member _types_, like `value_type`, so they can be used generically.
- `using` in this context is the same as `typedef`.
- We need `typename` to tell the compiler the following expression is a type.
  - This was not necessary for things like `vector<int>::iterator` since you can check that expression is a type.
  - Since `Container` here is a template parameter, the compiler cannot know what `Container::value_type` is until after template instantiation.

#### Unrelated note: templates can have default arguments. (e.g. `map`)

## Value template arguments

[`value.cpp`](value.cpp)
```c++
#include <iostream>

using namespace std;

template<typename T, int N>
class array
{
public:
    using value_type = T;

    T val[N];

    int size()
    {
        return N;
    }
};

template<int N>
int pow(int exponent)
{
    int val = 1;
    for (int i = 0; i < exponent; ++i)
    {
        val *= N;
    }

    return val;
}

int main()
{
    array<int, 10> t;
    cout << t.size() << "\n";

    cout << pow<2>(10) << "\n";
}
```

- We can also write class or function templates that are parameterized by values.
- e.g. `array` that takes the size of the array.
- e.g. `map` that can take a custom comparison function.

### `constexpr`

- What can we use as a value template argument?

[`constexpr1.cpp`](constexpr1.cpp)
```c++
#include <iostream>
#include <vector>

using namespace std;

template<int N>
int pow(int exponent)
{
    int val = 1;
    for (int i = 0; i < exponent; ++i)
    {
        val *= N;
    }

    return val;
}

int main()
{
    // note that if this code was const int base = 2; it would work.
    // This example is to illustrate that const is not sufficient to ensure something is known at compile time.

    // int i;
    // cin >> i;
    // const int base = i; // this is perfectly fine, we just can't change base later
    // cout << pow<base>(10) << "\n"; // but this isn't, since base needs to be known at compile time

    // constexpr expresses the property of being evaluated at compile time
    constexpr int base = 2;
    cout << pow<base>(10) << "\n";
}
```

- `constexpr` is a _stronger_ notion of immutability than `const`.
  - `const` is basically a _promise_ that you won't change this variable.
- A variable declared `constexpr` means it will be evaluated at compile time.
  - `constexpr` variables are also `const.
- Useful for declaring constants and placing values in read-only memory in the executable.
  - Similar to `#define`'s use cases in C.
- So far this seems kind of useless. Why do we need a whole new keyword for literals that the compiler knows at compile time anyways? Simple cases work just fine with `const`.
- What if we want to perform more complex operations at compile time?

[`constexpr2.cpp`](constexpr2.cpp)
```c++
constexpr long int collatz(long int i)
{
    if (i == 1)
        return 0;
    if (i % 2 == 0)
        return collatz(i / 2) + 1;

    return collatz(3 * i + 1) + 1;
}

int main()
{
    constexpr long int i = collatz(63728127);
}
```

- What if we want to perform more complex operations at compile time?
- `constexpr` can also be applied to functions.
- Functions can only be made `constexpr` if they are simple (pure in functional programming terminology): they can only use arguments passed to it and cannot have side effects.
- When a `constexpr` function is called with `constexpr` arguments, the output is also `constexpr`.
- A `constexpr` function can also be used as a regular function (which doesn't return a `constexpr`).

- `g++ -s -S constexpr2.cpp` to see the assembly output of the compiler.
  - `-s` removes debugging information, and `-S` generates assembly instead of an executable.

## Template metaprogramming

### Template specialization

[`specialization.cpp`](specialization.cpp)
```c++
#include <iostream>

template <typename T>
class wrapper
{
    T t;
public:
    wrapper(T t) : t {t} {}

    T get() const
    {
        return t;
    }

    void set(T t)
    {
        this.t = t;
    }
};

// Custom behavior for ints only where we ignore other operations and only return 1
template <>
class wrapper<int>
{
public:
    wrapper(int) {}

    int get() const
    {
        return 1;
    }

    void set(int) {}
};

using namespace std;

int main()
{
    wrapper w {12};

    cout << w.get();
}
```

- Templates can have custom code for specific instantiations.
- `template <>` is required, to tell the compiler the class/function is a template.
- The specializations must come after the general template declaration.
- e.g. `vector<bool>`

### Metaprogramming

- Metaprogramming is where programs can themselves deal with programs as data.
- Templates are already an instance of metaprogramming -- they generate code for you.

[`fib.cpp`](fib.cpp)
```c++
#include <iostream>

using namespace std;

template<int N>
struct fib
{
    static const int value = fib<N-1>::value + fib<N-2>::value;
};
template<>
struct fib<1>
{
    static const int value = 1;
};
template<>
struct fib<2>
{
    static const int value = 1;
};

int main()
{
    cout << fib<45>::value << "\n";
}
```

- A more general style that is actually Turing-complete (Veldhuizen 2003) was discovered, without the intent of the C++ designers.
- A "functional language" using templates, with specializations as "base cases".
- Moves computation from runtime to compile time.
- Uses `struct`s to hold values that we are computing.

[`collatz.cpp`](collatz.cpp)
```c++
#include <iostream>

using namespace std;

template<long int, long int> struct CollatzHelper;
template<long int> struct Collatz;

// A is even
template <long int A>
struct CollatzHelper<A, 0>
{
    static const long int steps = CollatzHelper<A/2, (A/2)%2>::steps + 1;
};

// A is odd
template <long int A>
struct CollatzHelper<A, 1>
{
    static const long int steps = CollatzHelper<(A*3)+1, (((A*3)+1)%2)>::steps + 1;
};

// terminator
template <>
struct CollatzHelper<1, 1>
{
    static const long int steps = 0;
};

template<long int A>
struct Collatz
{
    static const long int steps = CollatzHelper<A, A%2>::steps;
};

int main() {
    cout << "Collatz stopping time of 1: "
         << Collatz<1>::steps
         << endl;
    cout << "Collatz stopping time of 27: "
         << Collatz<27>::steps
         << endl;
    cout << "Collatz stopping time of 1729: "
         << Collatz<1729>::steps
         << endl;
    cout << "Collatz stopping time of 34969: "
         << Collatz<34969>::steps
         << endl;
    cout << "Collatz stopping time of 63728127: "
         << Collatz<63728127>::steps
         << endl;
}
```

- Similar in use to `constexpr`, though templates are more expressive.
  - E.g. templates can deal with any type, not just simple types.

## Abstracting over functions

- Higher order functions, as you've seen in CIS 120, are really useful.
- We can use function pointers from C, but these are fairly basic and cannot be parameterized (i.e. there are no function pointers to function templates).

### Function objects

[`functor.cpp`](functor.cpp)
```c++
#include <iostream>

using namespace std;

template <typename T>
class less_than
{
    T limit;
public:
    less_than(T limit) : limit {limit} {}

    bool operator()(T i) { return i < limit; }
};

// Expect p to be callable as a function T -> bool
template<typename P, typename T>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    less_than c1 {10};
    cout << algo(c1, 11) << " " << algo(c1, 9) << "\n";

    less_than c2 {"c"s};
    cout << algo(c2, "d"s) << " " << algo(c2, "a"s) << "\n";
}
```

- A function object (sometimes called a functor) is an object that can be called like a function.
- This is done by overloading `operator()`.
- The object can store other state inside, which can be useful.
- A functor is just a regular object, so its class can also be made a class template.
- Note that `struct`s and `class`es can be written within other structures, like functions and other `class`es.

[`hash.cpp`](hash.cpp)
```c++
#include <iostream>
#include <unordered_set>

using namespace std;

struct S
{
    string first_name;
    string last_name;

    bool operator==(const S& other) const
    {
        return first_name == other.first_name && last_name == other.last_name;
    }
};

// Writing our own functor to pass to an unordered_set
struct myHash
{
    size_t operator()(S const& s) const noexcept
    {
        size_t h1 = hash<string>{}(s.first_name);
        size_t h2 = hash<string>{}(s.last_name);
        return h1 ^ (h2 << 1);
    }
};

// Adding our template specialization to the std namespace
namespace std
{
    template<> struct hash<S>
    {
        size_t operator()(S const& s) const noexcept
        {
            size_t h1 = hash<string>{}(s.first_name);
            size_t h2 = hash<string>{}(s.last_name);
            return h1 ^ (h2 << 1);
        }
    };
}


int main()
{
    // using std::hash
    unordered_set<S> s1 { {"Paul", "He"} };

    // using myHash
    unordered_set<S, myHash> s2 { {"Paul", "He"} };
}
```

- `unordered_map` uses `std::hash` by default, or you can provide your own functor.
- `std::hash` is a template class defining a functor.
- The standard library provides specializations of `hash` for the basic C++ types.
- We can write our own specialization as well, and add it to the global namespace.

### Lambda expressions

[`lambda.cpp`](lambda.cpp)
```c++
#include <iostream>

using namespace std;

// Expect p to be callable as a function T -> bool
template<typename P, typename T>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    cout << algo([](int i){ return i < 10;}, 11) << " "
         << algo([](int i){ return i < 10;}, 9) << "\n";

    cout << algo([](string s){ return s < "c";}, "d"s) << " "
         << algo([](string s){ return s < "c";}, "a"s) << "\n";
}
```

- A lambda expression is shorthand for declaring a function object.

[`capture.cpp`](capture.cpp)
```c++
#include <iostream>

using namespace std;

// Expect p to be callable as a function T -> bool
template<typename P, typename T>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    int limit = 10;

    auto comparison = [&](int i)
                      {
                          return i < limit;
                      };

    cout << algo(comparison, 11) << " ";
    limit = 9; // This will only affect comparison if we captured by reference
    cout << algo(comparison, 9) << "\n";

    cout << algo([](string s){ return s < "c";}, "d"s) << " "
         << algo([](string s){ return s < "c";}, "a"s) << "\n";
}
```

- `[]` is the _capture list_ of the lambda expression.
  - It allows the body of the lambda to access variables outside it.
  - `[limit]` captures `limit` by value.
  - `[&limit]` captures `limit` by reference.
  - `[=]` and `[&]` capture _everything_ by value and reference respectively.
  - A comma delimited list can customize which variables are captures by value or by reference.
