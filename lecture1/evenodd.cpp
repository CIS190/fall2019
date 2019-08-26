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
