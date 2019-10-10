#include <iostream>
#include <deque>

using namespace std;

int main()
{
    deque<int> d;

    d.emplace_front(2);
    d.emplace_back(3);
    d.emplace_front(1);

    for (auto i : d)
        cout << i << "\n";

    cout << d[1] << "\n";
}
