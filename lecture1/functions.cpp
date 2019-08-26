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
