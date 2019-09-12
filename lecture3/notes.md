# Lecture 3: Sept 12 2019

## Separate compilation

- Code in multiple files (why?)
- Header files (`.h`, `.hpp` common) are the interface

[`separate/point.hpp`](separate/point.hpp)
```c++
#ifndef POINT_HPP
#define POINT_HPP

class point
{
private:
    int x;
    int y;

public:
    point(int, int);

    void move(int, int);
    double getDistance();
};

#endif

```

[`separate/point.cpp`](separate/point.cpp)
```c++
#include <cmath>

#include "point.hpp"

point::point(int x, int y) : x {x}, y {y}
{}

void point::move(int dx, int dy)
{
    x += dx;
	y += dy;
}

double point::getDistance()
{
    return sqrt(x * x + y * y);
}

```

[`separate/main.cpp`](separate/main.cpp)
```c++
#include <iostream>

#include "point.hpp"

using namespace std;

int main()
{
    point p {0, 0};
    cout << p.getDistance() << "\n";

    p.move(1, -1);
    cout << p.getDistance() << "\n";
}

```

- Use `#include "..."` for user-defined headers.
- `class::member(...)` (scope resolution operator) to _define_ member functions.
- Use include guards to prevent multiple `#include`s of the same file.
    - `#ifndef`, `#define`, `#endif`
  - `-E -P` to see the result of the preprocessor
- Each `.cpp` file (called a _translation unit_) is compiled into an object file, and these are all linked together to form the executable.
- Typically each class is in its own translation unit.

- Note that all this isn't strictly part of the language, it's just convention. It works fine to put definitions in headers (though functions defined in a class definition are `inline` by default.), or to put everything in one file. Compile times and ease of programming may suffer though (and your grades if you do this in homeworks).
  - And this system isn't perfect: e.g. the _definition_ of a class is put into the header file, including its private members.
  	- Because the private members are required to compute the size of the class (needed if you want to declare an object of this class, but _not_ if you use a pointer or reference)
	  - If a forward declaration of a class is sufficient use that, not the whole `#include` to speed up compile times. This is typically done in header files.
	- "pImpl" idiom to circumvent this
- "Real" module system to come in C++20 (`import module` instead of `#include`)

## More on classes

- Note that the parts of a class are typically called _members_ in C++, e.g. member functions, member data, though terms from other languages like _methods_ are sometimes used too.

### Constructors

- Default constructor: constructor with no arguments.
  - Required for some applications, like arrays.
  - When an object is constructed and a member isn't initialized, its default constructor is called.
  - If a class has _no_ constructors, an implicit one will be defined, which is equivalent to the empty constructor.
    - If you want the default-generated default constructor and it wasn't generated (because you have another constructor, for example), you can tell the compiler to do this explicitly: `classname() = default;`.
- You can also explicitly disallow use of the default constructor (and other implicitly generated things that we'll see later) with `classname() = delete;`.
  - Note the differing terminology from Java.

[`conversion.cpp`](conversion.cpp)
```c++
class list
{
public:
    list(int size) {}
};

void printList(list l)
{
    // ...
}

int main()
{
    printList(1);
}

```

- Single argument constructors define a conversion from its argument type
  - To disable this, use `explicit classname(...);`

### Misc things

[`default.cpp`](default.cpp)
```c++
#include <iostream>

using namespace std;

class point
{
public:
    int x = 0;
    int y = 0;

public:
    point() {}
    point(int x) : x {x} {}
    point(int x, int y) : x {x}, y {y} {}
};

int main()
{
    point p1;

    cout << p1.x << " " << p1.y << "\n";
}

```
- Default member initializers

[`const.cpp`](const.cpp)
```c++
#include <cmath>

class point
{
private:
	int x;
	int y;

public:
	point(int x, int y) : x {x}, y {y}
	{ }

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	double getDistance() const
	{
        // x++; // not allowed
		return sqrt(x * x + y * y);
	}
};

int main()
{
    const point p {0, 0};
    // p.move(1, 1); // not allowed
    p.getDistance();
}

```

- `const` after a member function declaration means the function does not mutate the object.
- Only `const` member functions can be used on `const` objects.
- Note that `const` is part of the type, so it must also be put on the definition in the `.cpp` file.

### Inheritance

[`inheritance.cpp`](inheritance.cpp)
```c++
#include <iostream>

using namespace std;

class animal
{
protected:
    string name;

public:
    animal(string name) : name {name}
    {}

    virtual void pet() const
    {}
};

class dog : public animal
{
public:
    dog(string name) : animal {name}
    {}

    void pet() const override
    {
        cout << name << " wags its tail.";
    }
};

class cat : public animal
{
public:
    cat(string name) : animal {name}
    {}

    void pet() const override
    {
        cout << name << " purrs.";
    }
};

void petLots(const animal & a)
{
    for (int i {0}; i < 10; ++i)
    {
        a.pet();
        cout << "\n";
    }
}

int main()
{
    dog d {"Rover"};
    cat c {"Strawberry"};

    petLots(d);
    petLots(c);
}

```

- `: public superclass`
- `virtual`
  - Not required in derived class, derived virtual members are still virtual
  - Why?
  - Overhead, pointer to vtable
- `override` after the member function declaration is not necessary
  - But it helps for documentation and can catch bugs (e.g. trying to override something but misspelled function name)
- Pure virtual function: `= 0;`
- Abstract classes are those that have a pure virtual function (either defined or inherited without being overriden with a non pure virtual function).
  - You cannot define objects of abstract classes.
- Classes you _can_ instantiate are called concrete classes.
- `final` can be used after the class name or after the function declaration to disallow inheritance and overriding, respectively.

## Other material

- `static` (these are the most common use cases)
  1. `static` local variables are basically global variables. They are initialized the first time they are used, but their values persist after the local scope ends.
  2. `static` class members are basically global functions/variables
	 - `static` member functions are basically a free (global) function in the class.
	 - `static` member variables are shared by all instances of the class.
	   - Note that you should still define these in the `.cpp`
  3. `static` global variables and functions are only accessible in the file they are defined in. This is more of a C thing, and if you want this in C++ it is more idiomatic to use an _anonymous namespace_: `namespace { local functions and global variables... } rest of file`

### Default function arguments


[`defaultargs.cpp`](defaultargs.cpp)
```c++
#include <iostream>

using namespace std;

void doSomething(int a, int i = 10)
{
    // ...
}

// note that you can no longer define the function
// void doSomething(int a)
// {
//     // ...
// }

int main()
{
    doSomething(10);
}

```

- It is possible to declare the default in either the declaration or the definition (not both). You should do this in the declaration (header), otherwise a caller can't know it exists.
- The default argument(s) must appear at the end of the argument list.

## "Bonus" unplanned material (_rough_ notes)
- `enum` and `enum class`es
- `enum` just declare some names for ints 0, 1, 2, 3, etc
- `enum class` is strongly typed and scoped enumerators
  - can actually define more operators, but won't go into this.
  - prefer enum class to enum, in most cases
