#include <iostream>
#include <vector>

using namespace std;

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

    int i;
    cin >> i;
    const int base = i;
    cout << pow<base>(10) << "\n";
}
