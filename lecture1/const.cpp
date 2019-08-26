#include <iostream>

using namespace std;

int main()
{
	const int a = 10;
	const int b = 11;

	// a++;

	const int * p {&a};
	p = &b;

	const int * const q {&a};
	// q = &b;

	cout << *p << " " << *q;
}
