# Lecture 2: Sep 5 2019

## References

- Why do we use pointers?
  - pass by reference
  - dynamic allocation
- _References_ will solve the first problem.

[`remainder.cpp`](remainder.cpp)
```c++
#include <iostream>

using namespace std;

void div(const int a, const int b, int & q, int & r)
{
	if (b == 0)
		return;

	q = a / b;
	r = a % b;
}

int main()
{
	auto q {0};
	auto r {0};

	div(9, 2, q, r);

	cout << q << " " << r << "\n";
}
```

- same e.g. as last class.
- Basically a `const` pointer that is automatically dereferenced.
- Must be initialized! No "null" reference.
- `const` references can be used for cheap argument passing
- note on spacing: `*` and `&` are part of the type, so usually written as `int*` or `int *`

## Standard streams

- stdout, stdin, stderr
  - abstraction of terminal I/O
  -	https://en.wikipedia.org/wiki/Standard_streams

## Output
[`buffered.cpp`](buffered.cpp)
```c++
#include <iostream>

using namespace std;

int main()
{
	// doesn't print due to buffering
	cout << "asdfasdf\n";
	abort();

	// the following 3 do work
	// cout << "asdfasdf" << endl;
	// abort();

	// cout << "asdfasdf\n";
	// cout.flush();
	// abort();

	// cerr << "asdfasdf\n";
	// abort();
}
```
- buffering

- objects of type `ostream`.
- `cout`
- `cerr` (not buffered)
- `clog` (buffered)

- `<<` chaining is OK because `<<` is left-associative and `<<` returns the same stream.
  - `cout << 1 << "2";` is the same as `(cout << 1) << "2";`

- You can also do some formatting, e.g. `cout << scientific << 1.1;`
  - many other operations available, some in the `<iomanip>` header.

### Input

[`cin.cpp`](cin.cpp)
```c++
#include <iostream>

using namespace std;

int main()
{
	string name;

	cout << "Enter your name: ";
	cin >> name;
	// getline(cin, name); // to read the entire line
	cout << "Hello " << name << "!\n";
}
```
- objects of type `istream`.
- `cin`
- `>>` ("get from")
  - skips whitespace
- `getline` to read until a newline.
- Handles memory management for you, no buffers to allocate.

[`readall.cpp`](readall.cpp)
```c++
#include <iostream>

using namespace std;

int main()
{
	string s;
	while (cin >> s)
	{
		cout << s;
	}
}
```

- streams have a conversion to `bool` which tell you whether there is an error (e.g. EOF)
  - alternatively use `cin.fail()`
  - Ctrl-D to insert an EOF when reading from stdin
- There are other flags and state operations for streams that I won't go into.
- `iostream` inherits from both `istream` and `ostream`

## File and string streams

### `<fstream>`

- streams to and from a file.

[`fstream.cpp`](fstream.cpp)
```c++
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	const string filename {"test.txt"};

	ofstream ofs {filename};
	if (!ofs)
		cerr << "oh no";
	ofs << "test";

	ifstream ifs {filename};
	if (!ifs)
		cerr << "oh no again";

	if (string s; ifs >> s)
		cout << s;
	else
		cerr << "nothing to read?";
}
```

- `ifstream`
- `ofstream`
- _Core_ stream operations are the same.
- There is also `fstream` that does both input and output, inherits from `iostream`

### `<sstream>`

- streams to and from a string.

```c++
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	ostringstream oss;
	oss << 1.1 << " " << scientific << 1.1;
	cout << oss.str() << "\n";

	istringstream iss {oss.str()};
	double a {0}, b {0};
	iss >> a >> b;
	cout << a << " " << b;
}
```

- `istringstream`
- `ostreamstream`
- `stringstream`
- Typically used if you want to read/write a string from something that's not a file or stdin/stdout (e.g. network, GUI, function call, etc).

### Polymorphism

[`polymorphism.cpp`](polymorphism.cpp)
```c++
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void printLogs(ostream & os)
{
	os << "something happened\n";
}

int main()
{
	ofstream ofs {"test.log"};
	ostringstream oss {"Log file:\n"};

	if (true)
		printLogs(cout);
	else if (true)
		printLogs(ofs);
	else
		printLogs(oss);
}
```

- Polymorphism in C++ requires passing by reference or pointer.
  - More details in a later lecture

## Classes

[`class.cpp`](class.cpp)
```c++
#include <iostream>
#include <cmath>

using namespace std;

class Point
{
private:
	int x;
	int y;

public:
	Point(int x, int y) : x {x}, y {y}
	{ }

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	double getDistance()
	{
		return sqrt(x * x + y * y);
	}
};

int main()
{
	Point p {0, 0};
	cout << p.getDistance() << "\n";

	p.move(1, -1);
	cout << p.getDistance() << "\n";
}
```

- `class` vs `struct`
  - don't need to declare a struct as `struct pair p`, just `pair p`
  - `struct`s are more "first class" than in C
  - "real" types
- The `Point` function is the _constructor_ for the class.
- `: x {x}, y {y}` is an _initializer list_, which initializes these variables. You can't initialize them in the body of the constructor, only assign to them. Since you can't assign to references, they _must_ be initialized in the initializer list.
- `this`: pointer to current instance, implicit first argument to member functions

## I/O for user-defined classes

[`overload.cpp`](overload.cpp)
```c++
#include <iostream>
#include <cmath>

using namespace std;

class Point
{
private:
	int x;
	int y;

public:
	Point(int x, int y) : x {x}, y {y}
	{ }

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	double getDistance()
	{
		return sqrt(x * x + y * y);
	}

	friend ostream & operator<<(ostream &, const Point &);
	friend istream & operator>>(istream &, Point &);
};

ostream & operator<<(ostream & os, const Point & p)
{
	os << p.x << ", " << p.y;

	return os;
}

istream & operator>>(istream & is, Point & p)
{
	string s;
	// note that this breaks if we input, e.g. 10,10. Consider is.get(char &) instead for the comma.
	is >> p.x >> s >> p.y;

	if (s != ",")
		is.setstate(ios_base::failbit);

	return is;
}

int main()
{
	Point p {1, -1};
	cout << p << "\n";

	if (cin >> p)
		cout << p << "\n";
}
```

- overload `operator>>` and `operator<<` for your classes
- idiomatic way of printing custom classes (vs `toString()` in Java)

- Return by reference
  -	Like pointers, be careful with scope
  - Here we are returning a reference that was also an argument
