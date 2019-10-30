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
    cout << algo([](int i){ return i < 10;}, 11) << " "
         << algo([](int i){ return i < 10;}, 9) << "\n";

    cout << algo([](string s){ return s < "c";}, "d"s) << " "
         << algo([](string s){ return s < "c";}, "a"s) << "\n";
}
