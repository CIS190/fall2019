#include <iostream>

using namespace std;

template<typename T, int N>
class array
{
public:
    using value_type = T;

    T val[N];

    int size()
    {
        return N;
    }
};

template<int N>
int pow(int exponent)
{
    int val = 1;
    for (int i = 0; i < exponent; ++i)
    {
        val *= N;
    }

    return val;
}

int main()
{
    array<int, 10> t;
    cout << t.size() << "\n";

    cout << pow<2>(10) << "\n";
}
