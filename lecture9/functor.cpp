#include <iostream>

using namespace std;

template <typename T>
class less_than
{
    T limit;
public:
    less_than(T limit) : limit {limit} {}

    bool operator()(T i) { return i < limit; }
};

// Expect p to be callable as a function T -> bool
template<typename P, typename T>
bool algo(P p, T i)
{
    return p(i);
}

int main()
{
    less_than c1 {10};
    cout << algo(c1, 11) << " " << algo(c1, 9) << "\n";

    less_than c2 {"c"s};
    cout << algo(c2, "d"s) << " " << algo(c2, "a"s) << "\n";
}
