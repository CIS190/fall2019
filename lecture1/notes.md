# Lecture 1: Aug 29 2019

First let's introduce some of the features of C++, most of which should be familiar from C.

## Smallest C++ program:
[`tiny.cpp`](tiny.cpp):
```c++
int main() {}
```

- Compile with `g++ -std=c++17 -Wall -g -o hello hello.cpp`.
- To use C++17 features, you should use `g++` version 7 or newer.
- `-Wall` turns all compilers warnings.
- `-g` adds debugging information to be used by the `gdb` debugger.
- `-o NAME` lets you specify the name of the output executable. Without it, the default name is `a.out`.
- `main` returns `int`

## Hello world

[`hello.cpp`](hello.cpp):
```c++
#include <iostream>

int main()
{
	std::cout << "Hello world\n";
}
```

### `#include`
- `#include` basically copy and pastes the contents of a file into the current file.
- `iostream` (input/output stream) contains the definitions of `std::cout` and `std::endl`.
- C++ standard library header files do not have the `.h` extension.
- "" vs <>

### `cout`
- `std::` is the standard library _namespace_.
- `using namespace std;` will "import" the names in the `std` namespace, and we will then be able to just use `cout` directly.
- `cout` (character output) is the standard output stream (we'll discuss in more detail later)
- `<<` is the "put to" operator (overloads the standard bit shift operator)
- `\n` is an _escape sequence_, which starts with `\`. It represents a newline character (0x0A in ASCII)
  - to print `\`, use `\\` in the string.

## Functions

[`functions.cpp`](functions.cpp):
```c++
#include <iostream>

using namespace std;

int dbl(int n)
{
	return 2 * n;
}

void print_dbl_int(int n)
{
	cout << "2 * " << n << " = " << dbl(n) << "\n";
}

int main()
{
	print_dbl_int(10);
}
```

- `void` means the function does not return any value.
- note that we can "chain" multiple `<<`s when using `cout`; we'll talk more about how this works later
- pass by value by default, always (unlike other languages)

We can overload functions (same function name) as long as they can be differentiated (e.g. adding `double dbl(int n)` is ambiguous since only the return type differs).
If we add:
```c++
double dbl(double n)
{
	return 2 * n;
}
```
to the code, everything will still work fine.

- Arithmetic operators: `+ - * / %`

C++ processes the source code linearly.
That means this code will not compile:

[`evenodd.cpp`](evenodd.cpp):
```c++
#include <iostream>

using namespace std;

bool even(int i)
{
    if (i == 0)
        return true;
    else if (i == 1)
        return false;
    else
        return odd (i - 1);
}

bool odd(int i)
{
    return even (i - 1);
}

int main()
{
    cout << even(9);
}
```

- We need to add a _declaration_ `bool odd(int);` to tell the compiler that this function exists, and has this type (return type + type and number of arguments).
- Declarations can happen multiple times, but _definitions_ can only happen once.

## If
- Comparison operators: `== != < > <= >=`
- Logical operators:
  - Bitwise: `& | ^ ~`
  - Logical: `&& || !`
- A _statement_ follows the `if`.
- Does `else if` exist?
- `if (int i = getVal(); i == 0) ...;`
- `switch`
  -	Like `if` and `for`, can also do `switch (int i = ...; i) ...`

## Variables
- e.g. `auto i {0};`
  - same as `int i = 0;`
  - `{}` can help detect unwanted type conversions: `int i = 1.1;` works but `int i {1.1};` doesn't.
- Initialize before use
  - Undefined behavior
- `const`
- Arrays
  - `int arr[4];`
  - `int arr[] {1, 2, 3, 4};`
  - `arr[0] = 1; ... arr[3] = 4;`
  - Does `arr[4] = 1;` work?
  	- Undefined behavior
- "Spiral rule"

## Loops

4 types of loops
```c++
for (auto i {0}; i < 10; ++i)
	...;
```

```c++
for (auto i : {1, 2, 3, 4})
	...;
```

```c++
while (i > 0)
	...;
```

```c++
do
{
	...
} while (i > 0);
```

- `i++` vs `++i` (or `--`)
- `+= -= *= /= %=`
- `,` operator
- scope

## Pointers

[`remainder.cpp`](remainder.cpp):
```c++
#include <iostream>

using namespace std;

void div(const int a, const int b, int * q, int * r)
{
	if (b == 0)
		return;

	*q = a / b;
	*r = a % b;
}

int main()
{
	int q {0};
	int r {0};

	div(9, 2, &q, &r);

	cout << q << " " << r << "\n";
}
```

- `*`
- Holds the address to some memory, using the type for size information
- `*` (contents of, also called dereferencing) and `&` (address of)

[`const.cpp`](const.cpp):
```c++
#include <iostream>

using namespace std;

int main()
{
	const int a = 10;
	const int b = 11;

	// a++;

	const int * p {&a};
	p = &b;

	const int * const q {&a}; // int const * const is also ok (and more consistent, though is uncommon)
	// q = &b;

	cout << *p << " " << *q;
}
```
- `const` pointers, "spiral rule" again

[`newdelete.cpp`](newdelete.cpp):
```c++
int main()
{
	auto * p {new int}; // auto p also works, but this is a bit more explicit
	*p = 10;
	delete p;

	auto * q {new int[10]};
	delete[] q;
}
```
- `new` and `delete`
  - `new[]` and `delete[]`
- Pointer arithmetic
  - Same mechanism as array access
- `sizeof`

- `nullptr`
- scope again

## Debugging

### `gdb`
[`debug.cpp`](debug.cpp):
```c++
void crash(int depth)
{
	int * p = nullptr;

	if (depth == 10)
		depth = *p;
	else
		crash(depth + 1);
}

int main()
{
	int a = 0;

	for (auto i {0}; i < 10; ++i)
		a++;

	crash(0);
}
```

- `gdb prog` to run gdb on the program `prog`
- or just run `gdb`, and load the program `prog` with `file prog`
- `run` to start debugging
- `backtrace` to view a stack trace
- `break filename:number` to set a breakpoint at some line number
  - or `break functionname` to set a breakpoint when the function is called
- `continue` to continue running after a breakpoint
- `step` to execute one line and break again
- `print varname` to print the value of a variable
- `watch varname` to break each time the value of a variable changes
- Many other features and editor integrations

### `valgrind`
```c++
int main()
{
	auto * p {new int};
}
```
