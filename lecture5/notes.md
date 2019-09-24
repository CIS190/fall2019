# Lecture 5: Sept 26 2019

## Exceptions
- In C...
  - Return codes
    - easy to ignore
  - `errno.h`
    - easy to ignore
  - `setjmp.h`
    - Like `goto`, can simulate exception handling in C, but won't support objects in C++.

[`exceptions.cpp`](exceptions.cpp)
```c++
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    try
    {
        throw 1;
        // string {"abc"}.substr(10);
        // auto i = new int [1000000000000];
        // throw "error message?";
    }
    catch (const int i)
    {
        cerr << i;
    }
    catch (const out_of_range & e)
    {
        cerr << "out_of_range: " << e.what();
    }
    catch (const exception e)
    {
        cerr << "exception: " << e.what();
    }
    catch (...)
    {
        cerr << "other\n";
        throw;
    }
}
```

- `throw` _unwinds_ the stack to find an exception handler (an enclosing `try`-`catch` block).
- We're allowed to `throw` any type.
  - `<stdexcept>` holds a base `exception` class and some common types of exceptions.
- Can `catch` by value or reference (but you should almost always catch by (const) reference, for polymorphism)
- `catch` clauses are processed in order from top to bottom. `catch(...)` catches any exception, so it has to be the last `catch` clause.
- No `finally` clause as in Java.
- `throw;` to rethrow the caught exception (useful for the `catch(...)` case, where it's unnamed).

[`noexcept.cpp`](noexcept.cpp)
```c++
#include <iostream>
#include <stdexcept>

using namespace std;

void f() noexcept
{
    throw 1;
}

int main()
{
    try
    {
        f();
    }
    catch (const int i)
    {
        cerr << i;
    }
}
```

- Can declare functions that should never throw as `noexcept`
  - The program terminates if the function does ever throw, even if a handler is available.

## Exception safety

- See the [wikipedia article](https://en.wikipedia.org/wiki/Exception_safety) for more details.
  1. No-throw guarantee
  2. Strong guarantee: failed operations are guaranteed to have no side effects. They are "rolled back".
  3. Basic guarantee: failed operations can result in side effects, but there are no leaks or broken invariants.
  4. No guarantee.

## Copy/move and swap

- If our `integer` example from last class had multiple members to copy/move, its `operator=` would only have a basic guarantee, since it may fail in the middle of the operation.

[`swap1.cpp`](swap1.cpp)
```c++
#include <iostream>
#include <utility>

using namespace std;

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        cout << "ctor\n";
    }

    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    integer(integer && i) : p {i.p}
    {
        i.p = nullptr;
        cout << "move ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(const integer & i)
    {
        integer temp {i}; // copy

        swap(p, temp.p);

        cout << "copy=\n";
        return *this;
    }
    integer & operator=(integer && i)
    {
        integer temp {move(i)}; // move

        swap(p, temp.p);

        cout << "move=\n";
        return *this;
    }

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

integer incr(integer i)
{
    i.set(i.get() + 1);
    return i;
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(i);

    cout << j.get() << "\n";
}
```

- `std::swap` in `<utility>` is `noexcept`.
- Only the call to the copy/move ctor can throw now.
- If the constructor fails, we don't perform the swap and the object is unchanged.
- This also removes the repeated code in the copy ctor/copy assignment, and in the move ctor/move assignment.

[`swap2.cpp`](swap2.cpp)
```c++
#include <iostream>
#include <utility>

using namespace std;

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        cout << "ctor\n";
    }

    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    integer(integer && i) : p {i.p}
    {
        i.p = nullptr;
        cout << "move ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(integer i)
    {
        swap(p, i.p);

        cout << "=\n";
        return *this;
    }

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

integer incr(integer i)
{
    i.set(i.get() + 1);
    return i;
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(i);

    cout << j.get() << "\n";
}
```

- Take the argument by value to skip the copy/move step, since it will be copied/moved in.

[`swap3.cpp`](swap3.cpp)
```c++
#include <iostream>
#include <utility>

using namespace std;

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        cout << "ctor\n";
    }

    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    integer(integer && i) : p {i.p}
    {
        i.p = nullptr;
        cout << "move ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(integer i)
    {
        using std::swap; // not needed since we already did using namespace std; above. If we didn't, we would need this for the fallback to std::swap to work.
        swap(*this, i); // since the arguments are in the integer namespace, this swap chooses integer::swap

        cout << "=\n";
        return *this;
    }

    friend void swap(integer & i, integer & j) noexcept
    {
        using std::swap;
        swap(i.p, j.p);
    }

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

integer incr(integer i)
{
    i.set(i.get() + 1);
    return i;
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(i);

    cout << j.get() << "\n";
}
```

- `std::swap` is implemented as 3 move assignments. If we try to do `swap(*this, i);` this will result in an infinite loop.
- But `swap` is a useful operation. The standard library `sort` uses it, for example.
- You can provide your own `swap` instead of `std::swap` for your class, and implement your move assignment in terms of that.
  - For some classes you may be able to write a faster version of `swap` than the 3 move assignments of `std::swap`
  - Your swap logic (here this is just swapping `p`) will have to go in either a custom `swap` or `operator=`.
- How does C++ choose which `swap` to use? If you specify using `std::swap`, it will pick that, but often we don't know if there's another `swap` that we want to use.
  - So typically `std::swap` is made accessible as `swap` with `using std::swap;`, and `swap` is called. A mechanism called [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl) looks for a `swap` related to the types of the arguments (`integer`) before other namespaces, so `swap`, a `friend` of `integer`, is called. If this `swap` didn't exist, `std::swap` would be called. Code in the standard library that calls `swap` does this, to use user-defined instances of `swap` before falling back to `std::swap`.
- `friend` declares the function as a friend and defines it at the same time. This is equivalent to declaring it as a friend in the class and defining it outside the class (which we did before with `operator<<` and `operator>>`).

- Note that there is overhead (use of temporary object, extra moves) to using copy/move and swap!

## Throwing constructors

[`ctor.cpp`](ctor.cpp)
```c++
#include <iostream>
#include <memory>

using namespace std;

class test
{
    int * p;
    int * q;

public:
    test()
        try : p {new int}
        , q {new int [10000000000000000]}
    {}
    catch (...)
    {
        delete p;

        cerr << "Caught in ctor\n";
    }

    ~test()
    {
        delete p;
        delete [] q;
        cerr << "dtor\n"; // never called since test was never successfully constructed
    }
};

int main()
{
    try
    {
        test t;
    }
    catch (...)
    {
        cerr << "Caught in main\n";
    }
}
```

- If something goes wrong in a constructor, you should throw an exception, since there is no return value.
- Note that if a constructor exits by a throw, the destructor is _not_ called.
- To catch exceptions thrown by the initializer list, use a _function try block_ which catches exceptions from the function body and the initializer list.
  - The catch clause(s) of a function try block _must_ throw an exception. If you don't throw one, it will rethrow automatically.
  - You can also use function try blocks for regular functions, but they are really only useful for constructors. You can read more about it [here](https://en.cppreference.com/w/cpp/language/function-try-block)
- The code above doesn't really work that well. We know it's `q` that's throwing, and that allocating `p` succeeds, but what if we don't? We have to delete the one(s) that were allocated successfully. Which do we delete?

## Smart pointers

- C++ guarantees that constructed objects get their destructors called when they go out of scope, even when this is due to an exception.
- A _smart pointer_ is a class that wraps a regular pointer, and uses its destructor to `delete` it.

### `unique_ptr`

[`smartctor.cpp`](smartctor.cpp)
```c++
#include <iostream>
#include <memory>

using namespace std;

class test
{
    unique_ptr<int> p;
    unique_ptr<int []> q;

public:
    test()
        try : p {make_unique<int>(0)}
        , q {make_unique<int []>(10000000000000000)}
    {}
    catch (...)
    {
        cerr << "Caught in ctor\n";
    }

    ~test()
    {
        cerr << "dtor\n"; // never called since test was never successfully constructed
    }
};

int main()
{
    try
    {
        test t;
    }
    catch (...)
    {
        cerr << "Caught in main\n";
    }
}
```

- `unique_ptr` (in `<memory>`) represents unique ownership of a pointer.
- The type held is enclosed in `<>`.
- Can be initialized directly with `unique_ptr<int> i {new int {1}};`, or using `auto` and `make_unique`: `auto i {make_unique<int>(1)};`. `make_unique` allows us to use `auto` and avoid repeating the type twice. It also calls the constructor of the type based on the arguments passed to it, so we don't need to call `new` ourselves.
  - `make_unique` also makes sure that the thing being put into the `unique_ptr` is a newly allocated piece of memory, and not any random pointer, for example.
- If the `unique_ptr` holds an array, its type can be specified as `<int []>`.
- All the operations (`*`, `->`, `[]` in the case of `<T[]>` pointers) on pointers work the same way.
- `get`, `reset`, and `release` deal with the underlying pointer.
- It's possible to specify a custom deleter, to call `fclose` on a `FILE *`, for example.

[`unique.cpp`](unique.cpp)
```c++
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

class test
{
public:
    ~test()
    {
        cerr << "~test\n";
    }
};

unique_ptr<test> f(unique_ptr<test> p)
{
    return p;
}

int main()
{
    auto p {make_unique<test>()};
    auto q {make_unique<test>()};

    // p = q; // doesn't work
    p = move(q);

    // p = f(q); // doesn't work
    p = f(move(q));

    assert(!q); // q is invalidated after the move
}
```

- `unique_ptr`s cannot be copied, only moved.

### `shared_ptr`

[`shared.cpp`](shared.cpp)
```c++
#include <iostream>
#include <memory>

using namespace std;

class test
{
public:
	void foo()
	{
		cerr << "foo\n";
	}

    ~test()
    {
        cerr << "~test\n";
    }
};

int main()
{
    auto t1 {make_shared<test>()};
	auto t2 {t1};

	t1.reset();

	t2->foo();

	t2.reset();

	cerr << "shared_ptrs both cleared\n";
}
```

- `shared_ptr` represented shared ownership of a pointer.
- `shared_ptr`s can be copied, and keep track of the number of references each held object.
  - Use the `shared_ptr`'s copy constructor or assignment operator. Making a new `shared_ptr` using the underlying pointer is undefined behaviour.
- An object is only destroyed once no `shared_ptr`s hold that object.

### `weak_ptr`

[`weak.cpp`](weak.cpp)
```c++
#include <iostream>
#include <memory>

using namespace std;

class test
{
public:
	// shared_ptr<test> other; // never destroyed if there is a cycle
	weak_ptr<test> other;

	void foo()
	{
		if (auto shared = other.lock())
		{
			// do something with the shared_ptr to other
			cerr << "obtained shared_ptr to other\n";
		}
	}

    ~test()
    {
        cerr << "~test\n";
    }
};

int main()
{
	{
		auto t1 {make_shared<test>()};
		auto t2 {make_shared<test>()};

		t1->other = t2;
		t2->other = t1;

		t1->foo();
	}

	cerr << "shared_ptrs both out of scope\n";
}
```

- `weak_ptr` represents temporary ownership of a pointer ordinarily owned by a `shared_ptr`.
- Without `weak_ptr` objects with cyclic references would never be destroyed.
- Must be converted to a `shared_ptr` temporarily to access the underlying pointer using `lock`.
- Since `lock` returns a `shared_ptr`, it increases the reference count by one and the pointer will not be invalidated while it is being used.

## Resource acquisition is initialization (RAII)

- Idiom where resources are attached to the lifetime of the object. Acquiring resources (memory, files, locks, etc) is done in the constructor, and released in the destructor. If acquiring the resource fails, the constructor fails, and the object is invalid. Otherwise, the language guarantees that the destructor is called, releasing the resource and preventing leaks.
- _Crucial_ in C++. Takes the place of `finally` in other languages.
