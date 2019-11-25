#include <iostream>

using namespace std;

struct A
{
    unsigned char c[4] {1, 0, 1, 0};
};

struct B
{
    unsigned int i;
};

int main()
{
    A a;
    // B b {static_cast<B &>(a)}; // static_cast does a check
    B b {reinterpret_cast<B &>(a)};

    cout << b.i;
}
