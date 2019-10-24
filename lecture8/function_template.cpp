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
