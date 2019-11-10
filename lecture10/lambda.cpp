#include <iostream>

using namespace std;

int main()
{
    auto f = [](auto a, auto b)
    {
        return a + b;
    };

    cout << f(1, 2) << "\n";
    cout << f("Hello "s, "world") << "\n";
}
