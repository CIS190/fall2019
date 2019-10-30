#include <iostream>

using namespace std;

// Expect p to be callable as a function T -> bool
template<typename P, typename T>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    int limit = 10;

    auto comparison = [&](int i)
                      {
                          return i < limit;
                      };

    cout << algo(comparison, 11) << " ";
    limit = 9; // This will only affect comparison if we captured by reference
    cout << algo(comparison, 9) << "\n";

    cout << algo([](string s){ return s < "c";}, "d"s) << " "
         << algo([](string s){ return s < "c";}, "a"s) << "\n";
}
