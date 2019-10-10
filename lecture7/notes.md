# Lecture 7: Oct 17 2019

- A container is an object that holds a collection of other objects.
- Today we will only talk about _sequence_ containers, where the user specifies the order.

## `vector`

[`vector.cpp`](vector.cpp)
```c++
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;

    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    for (unsigned int i = 0; i < v.size(); ++i)
        cout << v[i] << "\n";

    for (auto i : v)
        cout << i << "\n";
}
```

- `vector` provides an array-like contiguous sequence of elements.
- `push_back` adds an object to the end of the `vector`.
- All containers provide `.size()` and `.empty()`, among other members.
- Note that you should use a `vector` of `unique_ptr` or something similar if you want polymorphic behavior.

[`emplace.cpp`](emplace.cpp)
```c++
#include <iostream>
#include <vector>

using namespace std;

class test
{
public:
	test() { cerr << "ctor\n"; }
	test(const test &) { cerr << "copy ctor\n"; }
	test(test &&) noexcept { cerr << "move ctor\n"; }
	~test() { cerr << "dtor\n"; }
};

int main()
{
    vector<test> v;

    for (int i = 0; i < 10; ++i)
    {
        // v.push_back(test {}); // unnecessary move constructor needed
		v.emplace_back();
    }
}
```

- `vector` is typically implemented by doubling its size once it is full.
- Lots of copies/moves are made!
  - `vector` prefers copies/moves in this order: `noexcept` move ctor, copy ctor, normal move ctor (since a move ctor that throws is more dangerous than a copy ctor that throws, see [this SO question](https://stackoverflow.com/questions/28627348/noexcept-and-copy-move-constructors) for details)
- `push_back` copies/moves an object into the `vector`.
- `emplace_back` takes the arguments of the object constructor and creates the object _in_ the `vector`, saving a copy/move

[`range.cpp`](range.cpp)
```c++
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;

    for (int i = 0; i < 10; ++i)
        v.emplace_back(i);

    cout << v[0] << "\n";
    cout << v[10] << "\n";

    try
    {
        cout << v.at(10) << "\n";
    }
    catch (const out_of_range &)
    {
        cout << "Out of range access\n";
    }
}
```

- Accessing elements using `[]` just accesses the underlying memory, without range checking.
- Using `.at(...)` to access an element does range checking, and throws `out_of_range` if the argument is out of range.
  - There is some overhead to doing this check (~10%).

[`operations.cpp`](operations.cpp)
```c++
#include <iostream>
#include <vector>

using namespace std;

class test
{
public:
    bool operator==(const test & t) const
    {
        return true;
    }

    bool operator<(const test & t) const
    {
        return true;
    }
};

int main()
{
    vector<int> v1 {1, 2, 2};
    vector<int> v2 {1, 2, 3};

    cout << (v1 == v2) << "\n";
    cout << (v1 < v2) << "\n";

    swap(v1, v2);
    for (int i : v1)
        cout << i << "\n";

    v1 = move(v2);
    for (int i : v1)
        cout << i << "\n";
}
```

- All containers provide a few standard operations.
  - `operator==` just checks if all the members are equal and in the same order.
  - `operator<` checks the lexicographic order.
  - `swap`, copy/move constructors/assignments are all defined.
- Aside: `vector` has a constructor that takes the size of the vector to create. How can we initialize a `vector` to be a specific vector of size 1?
  - `{1, 2, 3}` is of type `initializer_list`, so we can force this using vector<int> v {{1}};

## `array`

[`array.cpp`](array.cpp)
```c++
#include <iostream>
#include <array>

using namespace std;

int main()
{
    int bad[] {1, 2, 3};

    // C-style arrays are prone to subtle bugs like this
    for (int i = 0; i <= 3; ++i)
        cout << bad[i] << "\n";

    // In C++17 the type and size can be deduced if the array is initialized
    array<int, 3> {1, 2, 3};
    for (unsigned int i = 0; i < arr.size(); ++i)
        cout << arr[i] << "\n";

    // Bound-checked access is also available
    for (unsigned int i = 0; i <= 3; ++i)
        cout << arr.at(i) << "\n";

    // We can also access elements like this, since the size is fixed.
    // This check is at _compile_ time!
    cout << get<0>(arr) << "\n";

    // cout << get<3>(arr) << "\n"; // Won't compile
}
```

- A container for a fixed-length array, an alternative to using C arrays.
  - Benefits are that it stores its size (using `.size()`), doesn't convert to a pointer implicitly, and has some convenience functions.
  - _No_ runtime overhead compared to C arrays.
- Defined in `<array>`.

## `list`

[`list.cpp`](list.cpp)
```c++
#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l;

    l.emplace_front(2);
    l.emplace_back(3);
    l.emplace_front(1);

    for (auto i : l)
        cout << i << "\n";
}
```

- `list` is a doubly-linked list.
- `forward_list` is a singly-linked list, if you really need the lower overhead.
  - It also does not provide `size()` to reduce overhead.

## `deque`

[`deque.cpp`](deque.cpp)
```c++
#include <iostream>
#include <deque>

using namespace std;

int main()
{
    deque<int> d;

    d.emplace_front(2);
    d.emplace_back(3);
    d.emplace_front(1);

    for (auto i : d)
        cout << i << "\n";

    cout << d[1] << "\n";
}
```

- A double ended queue (deque, pronounced "deck") has 2 usual implementations: as a doubly linked list and as an array that can be extended in either direction. Since C++ provides a linked list container already, `deque` is an implementation of the latter.
- Like `vector`, but also with fast insertion at front.
  - Not strictly better however, since this is implemented with multiple underlying arrays (we can't extend arrays in both directions indefinitely). Multiple pointer dereferences are needed to access elements, as opposed to one dereference for a `vector`. It also typically needs more memory than a `vector`.

## Container adapters

- C++ also provides container _adapters_, which are containers that use another container as its underlying container.
- `stack` and `queue` are fairly straightforward. Both use `deque` by default, but `list` also works and `vector` is also suitable for `stack`.
- `priority_queue` provides constant time for accessing the largest element, and logarithmic time for insertion and deleting the largest element.
  - Uses `vector` by default, though `deque` is also available.
  - Uses standard library functions to manage a heap in the underlying container.
  - The comparison function can be customized to reverse the ordering, for example.

| Container | Insertion | Access | Deletion |
| --- | --- | --- | --- |
| `vector` | O(1) at back<br>O(n) otherwise | O(1) | O(1) at back<br>O(n) otherwise |
| `deque` | O(1) at front and back<br>O(n) otherwise | O(1) | O(1) at front and back<br>O(n) otherwise |
| `list`/`forward_list` | O(1) after reaching the element | O(n) | O(1) after reaching the element |
| `priority_queue` | O(log n) | O(1) for the max | O(log n) for the max |

## Iterators

- But wait, how do we actually use a `list` besides accessing the front and back?

[`iterator1.cpp`](iterator1.cpp)
```c++
#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};

    for (list<int>::iterator i = l.begin(); i != l.end(); ++i)
        cout << *i << "\n";
}
```

- Iterators are objects that denote an element from a sequence of elements.
- Containers all have a type member `::iterator` (and `::const_iterator`) for the type of its iterator.
- All containers have the `begin()` and `end()` (and `cbegin()` and `cend()` for const iterators) member functions to expose iterators to the beginning of the sequence and _one past the end_ of its sequence, respectively.
  - This is what the range based for loop (`for (int i : l)`) uses.
- To use iterators, dereference them using `*` or `->`. This doesn't mean iterators are pointers (though sometimes they are!).

[`iterator2.cpp`](iterator2.cpp)
```c++
#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};
    vector<int> v {1, 2, 3, 4};

    // cout << *(l.begin() + 2) << "\n"; // lists do not provide random access iterators
    cout << *(v.begin() + 2) << "\n";
}
```

- There is a hierarchy of iterator types, though iterators don't use a class hierarchy.
  - They use template arguments instead (next lecture) for performance.
- Here is a simplified description of the hierarchy:
  - All iterators can increment (`++`) through its sequence, be compared for equality (`==`, `!=`), and access its element of the sequence (`*`, `->`). If they are not `const` iterators, they can also be assigned to (`*it = ...`). Iterators that can _only_ do this are _forward iterators_.
    - e.g. `forward_list`
  - Bidirectional iterators can also decrement (`--`)
    - e.g. `list`
  - Random access iterators can go forwards and backwards any number of steps in constant time (`+`, `-`, `+=`, `-=`), and can be compared for inequality (`<`, `<=`, `>`, `>=`).
    - e.g. `vector`, `deque`
- Note that pointers are valid random access iterators (which also means they are valid instances of all the possible types of iterators).
- Note that also some of these operations can be simulated.
  - e.g. `advance(it, n)` in `<iterator>` just performs `++` on `it` `n` times.
    - Note that random access iterators can do this in constant time, while forward iterators will need linear time.
  - `distance(first, last)` returns the number of `++` required to get from `first` to `last`. If the iterators are not random access iterators, is undefined behavior if `last` is not reachable from `first`.

[`iterator3.cpp`](iterator3.cpp)
```c++
#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};
    vector<int> v {1, 2, 3, 4};

    {
        auto it = l.begin();
        it++;
        l.insert(it, 0); // inserts before the iterator
        cout << "it: " << *it << "\n";

        for (int i : l)
            cout << i << "\n";
    }

    {
        auto it2 = v.begin() + 2;
        cout << "it2: " << *it2 << "\n";

        auto it = v.begin();
        it++;
        v.insert(it, 0); // if the vector size changes, all iterators are invalidated.
        cout << "it: " << *it << "\n";

        // iterators that are after the modified element are always invalidated for vectors.
        cout << "it2: " << *it2 << "\n";

        for (int i : v)
            cout << i << "\n";
    }
}
```

- All sequence containers provide `insert`, which inserts before an iterator.
- Iterators can be _invalidated_ when the container changes. This depends on the container.
- See [chart](https://en.cppreference.com/w/cpp/container) for a summary of the invalidation rules

[`iterator4.cpp`](iterator4.cpp)
```c++
#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l {1, 2, 2, 4};

    for (auto it = l.begin(); it != l.end(); )
    {
        if (*it == 2)
        {
            // erase returns an iterator to the next element past the erased element
            it = l.erase(it);

            // l.erase(it); // it is invalidated here!
        }
        else
        {
            ++it;
        }
    }

    for (int i : l)
        cout << i << "\n";
}
```

- All sequence containers provide `erase` as well, which deletes the element at an iterator.
- `array` iterators never get invalidated, since the size cannot change.
