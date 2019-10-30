#include <iostream>
#include <vector>
#include <list>

using namespace std;

// in vector/list/etc
// using value_type = T;
// same as typedef T value_type;

template <typename Container>
typename Container::value_type sum(const Container & c)
{
    // need typename to tell C++ this is a type
    typename Container::value_type total {};
    for (auto & i : c)
    {
        total += i;
    }

    return total;
}

int main()
{
    vector<int> v {1, 2, 3};
    list<string> l {"h", "e", "l", "l", "o"};

    cout << sum(v) << "\n";
    cout << sum(l) << "\n";
}
