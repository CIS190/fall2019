# Lecture 8: Oct 24 2019

## Stream iterators

[`ostream.cpp`](ostream.cpp)
```c++
#include <iostream>
#include <iterator>

using namespace std;

int main()
{
    ostream_iterator<string> oi {cout};

    *oi = "Hello ";
    oi++; // no-op
    *oi = "world"; // The * is also a no-op
}
```

- Iterators are elements of _sequences_, not containers.
- Iterators can be made from streams as well.
- `ostream_iterator` can write to an `ostream` with the same interface as a container.
  - `ostream_iterator` cannot be read from. What I said earlier about all iterators having `*` and `->` was a simplication. There are _input_ and _output_ iterators, and `ostream_iterator`s are output iterators only.

[`istream.cpp`](istream.cpp)
```c++
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    istream_iterator<string> ii {cin};
    istream_iterator<string> eos; // end of stream iterator

    cout << *ii << "\n";
    ii++;
    cout << *ii << "\n";
    cout << *ii << "\n";

    ii++;
    vector<string> v;
    v.insert(v.begin(), ii, eos);

    for (auto & s : v)
        cout << s << " ";
}
```

- Each time an `istream_iterator` is incremented, it will read from its stream.
- When it is dereferenced, it will return the value last read.
- The default-constructed `istream_iterator` represents the end-of-stream iterator.

## Associative containers

- Containers that store a collection of key-value pairs (or something similar), which are not ordered by the user.

### First: `pair`

[`pair.cpp`](pair.cpp)
```c++
#include <iostream>
#include <utility>

using namespace std;

int main()
{
    pair<int, string> p1 {1, "hello"};
    // auto p2 {1, "hello"}; // doesn't work, could be list, vector, etc.
    auto p2 {make_pair(1, "hello")};

    cout << p1.first << " " << p2.second;
}
```

- Combines two objects of any type.
- In `<utility>`.
- Use `make_pair` to avoid typing out the types.
- Access the elements using `.first` and `.second` (note these are member _objects_, not member _functions_).
- Generalized by `tuple`.
- One possible implementation for an associative container is just a sequence container of `pair`s.

### `map`

[`map1.cpp`](map1.cpp)
```c++
#include <iostream>
#include <map>

using namespace std;

int main()
{
    map<int, string> numbers;
    numbers[0] = "zero";
    numbers[1] = "one";

    map<int, string> numbers2 {{2, "two"}, {3, "three"}};

    cout << numbers[0] << " " << numbers[1] << " " << numbers2[2] << "\n";
    cout << numbers[2]; // creates the element if it doesn't exist

    cout << numbers.size();
}
```

- Typically implemented using a balanced BST like a red-black tree.
- `[]` returns a reference to the value corresponding to a key, if the key exists in the map. If the key doesn't exist in the map, it will be created.
- Other familiar member functions are also available:
  - `at` accesses a key's value. If the key doesn't exist, an exception is thrown.
  - `empty`, `size`, `clear`, `erase`.
  - `insert` and `emplace` are just like the functions on sequential containers, except they take a `pair` (no iterator required, since we don't need to manage the order ourselves).
- Iterating through the container will always go in sorted order.

[`map2.cpp`](map2.cpp)
```c++
#include <iostream>
#include <map>

using namespace std;

class test
{
public:
    int i;

    test(int i) : i {i} {}

    bool operator<(const test & o) const
    {
        return i < o.i;
    }

    bool operator>(const test & o) const
    {
        return !(*this < o);
    }
};

int main()
{
    map<test, string> m;

    test t1 {1};
    test t2 {2};
    test t3 {3};

    m[t1] = "one";
    m[t2] = "two";
    m[t3] = "three";
    m[4] = "four"; // implicit conversion also works

    for (auto & p : m)
    {
        cout << p.first.i << " " << p.second << "\n";
    }
}
```

- If your key is a custom class, you'll need to specify an ordering to use.
  - By default `map` uses `<`.
  - You can customize this by passing in a comparison _function object_ (more on this later) as a template argument.
    - The standard library has a few built in: `less<Key>`, which uses `operator<` for your class, is the default.
    - Others include `greater<Key>`, `less_equal<Key>`, and `greater_equal<Key>`.
- `map` doesn't use `operator==` to check for equality; we _only_ require an ordering.
  - If `!(a < b) && !(b < a)` then `map` considers `a` and `b` equivalent.

### `unordered_map`

[`unordered_map.cpp`](unordered_map.cpp)
```c++
#include <iostream>
#include <unordered_map>

using namespace std;

int main()
{
    unordered_map<int, string> numbers;
    numbers[0] = "zero";
    numbers[1] = "one";

    unordered_map<int, string> numbers2 {{2, "two"}, {3, "three"}};

    cout << numbers[0] << " " << numbers[1] << " " << numbers2[2] << "\n";
    cout << numbers[2]; // creates the element if it doesn't exist

    cout << numbers.size();
}
```

- `map` uses a BST, with logarithmic complexity for its operations.
- Hash tables can do better, and that's what `unordered_map` is.
  - Average constant time complexity of its operations.
- Otherwise, `unordered_map` has basically the same interface as `map`.
- What should we do for custom key types?

### `hash`

- Other languages have hash codes built in to the language.
  - This requires a common base class.
  - But not everything in C++ is an object!
  - Also a virtual call may be required if we use a hash code method. This forced overhead is really not in the spirit of C++.
- This also uses a comparison object (so we'll talk about it in a later lecture).
- `unordered_map` also needs `operator==`, to deal with hash collisions.

### `set` and `unordered_set`

- These containers are basically the `map` and `unordered_map` containers, but without a value attached to the keys.

### `multi` variants

- There are also the `multimap`, `unordered_multimap`, `multiset`, and `unordered_multiset` classes for associative containers that can have multiple copies of a key.
- These containers can return a sequence of results (a start and end iterator).

## Templates

### Class templates

[`class_template.cpp`](class_template.cpp)
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

using namespace std;

int main()
{
    wrapper w {12};

    cout << w.get();
}
```

- Templates are used for _parametric polymorphism_, for writing code that does not depend on the type of a value.
- We can write classes that are parametric on an input type.
- The container classes and smart pointers that we've seen are all examples of these.

[`templates/wrapper.hpp`](templates/wrapper.hpp)
```c++
#ifndef WRAPPER_HPP
#define WRAPPER_HPP

template <typename T>
class wrapper
{
    T t;
public:
    wrapper(T t);

    T get() const;
    void set(T t);
};

#include "wrapper.tpp"

#endif
```

[`templates/wrapper.tpp`](templates/wrapper.tpp)
```c++
template <typename T>
wrapper<T>::wrapper(T t) : t {t}
{}

template <typename T>
T wrapper<T>::get() const
{
    return t;
}

template <typename T>
void wrapper<T>::set(T t)
{
    this.t = t;
}
```

- The compiler generates different code each time the template class is _instantiated_, so the implementation and interface must be bundled together in the same file for this to occur (due to separate compilation).
  - Sometimes people still break up the interface and implementation for organization or design reasons, but `#include` the implementation file (usually using a different file extension) in the interface file so it still compiles.
- `typename` and `class` are interchangeable in the template prefix. I prefer `typename`, since not all types are classes in C++. You may find `class` in older code.

### Function templates

[`function_template.cpp`](function_template.cpp)
```c++
#include <iostream>

using namespace std;

template <typename T>
void my_swap (T & a, T & b)
{
    T c {a};
    a = b;
    b = c;
}

int main()
{
    int i {1};
    int j {2};

    my_swap(i, j);

    cout << i << " " << j;
}
```

- We can also write function templates.
- The things we've talked about in the past 2 weeks are _not_ object-oriented.
  - If it were, there would be overhead in the form of virtual function calls when trying to perform operations abstractly.
  - To abstract over them we can use templates instead, since they share a consistent common interface.
