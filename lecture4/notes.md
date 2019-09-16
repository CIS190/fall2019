# Lecture 4: Sept 19 2019

Note from before: `complex c {};` is _implictly_ `complex c; c.complex();`.
And `complex c {1, 2};` is implictly `complex c; c.complex(1, 2);`.
Note that you _can_ call constructors explictly on already-initialized objects.

Common error: trying to initialize an object using its default constructor like `complex c();`.
This parses to a function declaration named `c`, returning a `complex` and taking no arguments.
Use `complex c;` or `complex c {};` instead.

## Classes that hold resources

[`eg0.cpp`](eg0.cpp)
```c++
#include <iostream>

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

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

int main()
{
    integer i {1};
    integer j {2};
    integer k {i.get() + j.get()};

    cout << k.get() << "\n";
}
```

## Destructors

[`eg1.cpp`](eg1.cpp)
```c++
#include <iostream>

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

    ~integer()
    {
        delete p;
        cout << "dtor\n";
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

int main()
{
    integer i {1};
    integer j {2};
    integer k {i.get() + j.get()};

    cout << k.get() << "\n";
}
```

- Called automatically when an object's lifetime ends
- Used to clean up resources
- If there is a possibility of subclasses, destructors should be `virtual` (why?)
- Can call `i.~integer();` manually, but you shouldn't (and should have no reason to) do this, since it will be called again when its lifetime ends, and can result in a double `delete`.

### Order of initialization and deinitialization

```c++
#include <iostream>
using namespace std;

class test
{
public:
    int a;
    int b;

    // compiler warning about order
    test() : b {1}, a {b}
    {}

};

int main()
{
    test t;
    cout << t.a;
}
```

- The constructor initializes the base class (using a constructor if it was in the initializer list), then class members in _declaration_ order (order in initializer list doesn't matter), then the constructor body.
- The destructor calls the destructor body, then destructs class members in _reverse_ order of declaration, then destructors of base classes (why?).
- Note that we use declaration order, since the order of the initializer list can differ for different constructors, and the destructor needs to run in the reverse order as construction.
- Destruction order in general is the reverse of the construction order (e.g. local variables).

## Copy

[`eg2.cpp`](eg2.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }

    ~integer()
    {
        delete p;
        cout << "dtor\n";
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

int main()
{
    integer i {1};
    integer j {2};
    integer k {i};

    cout << k.get() << "\n";
}
```

[`eg3.cpp`](eg3.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }

    ~integer()
    {
        delete p;
        cout << "dtor\n";
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

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {incr(i)};

    cout << j.get() << "\n";
}
```

- Copies are made when:
  - Initialization with another object: `integer i {j}`, `integer i(j)`, `integer i = j`.
  - Passing an object (by value) to a function.
  - Returning an object (by value) from a function.
  - Throwing an object as an exception (more on this in a later lecture).
  - Assigning an object to another object (more on this in the next example).
- `integer i {j};` is implictly `integer i; i.integer(j);`
- The parameter to the copy ctor must be a reference to its own class, but doesn't have to be `const`. Would a taking a non-referencemake sense?
- Copy elision omits copies in some simple cases.

[`eg4.cpp`](eg4.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(const integer & i)
    {
        *p = i.get();
        cout << "copy=\n";
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

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {2};
    j = i;

    cout << j.get() << "\n";
}
```

- Assignment
- Be careful of self assignment
  - e.g. if in `operator=` we have `delete p; p = new int {i.get()};` this would fail for self assignment!

## Move

[`eg5.cpp`](eg5.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
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
        *p = i.get();
        cout << "copy=\n";
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

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {incr(i)};

    cout << j.get() << "\n";
}
```

- lvalues vs rvalues
- note that `delete nullptr;` is a NOP.

[`eg6.cpp`](eg6.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
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
        *p = i.get();
        cout << "copy=\n";
        return *this;
    }
    integer & operator=(integer && i)
    {
        p = i.p;
        i.p = nullptr;
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

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(i);

    cout << j.get() << "\n";
}
```

- Assignment again
- Self assignment? (see below)

[`eg7.cpp`](eg7.cpp)
```c++
#include <iostream>

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

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
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
        *p = i.get();
        cout << "copy=\n";
        return *this;
    }
    integer & operator=(integer && i)
    {
        p = i.p;
        i.p = nullptr;
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

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(move(i));

    cout << j.get() << "\n";
}
```

- `std::move` in `<utility>`
- Forcefully moving
- Use in move ctor and move assn, to move member variables with user-defined types, for example.
- Note that you can assume that `i = move(i)` is not performed, so move assn doesn't have to worry about self assignment.

## What if there's a base class?

- Note that you need to call the base class copy/move constructor/`operator=` yourself if you are writing a custom copy/move constructor/`operator=` (and you want the base class's behaviour). The implicit (`= default`) ones will do it for you.
  - All constructors (copy/move included) will call the base class default constructor if you don't call any base class constructor in the initializer list.
  - Destructors will also call the base class destructor (as noted above)
