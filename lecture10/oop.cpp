#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

class test
{
public:
    void print()
    {
        cout << "print()\n";
    }
};

int main()
{
    vector<test> v (10);

    // We'd like to do something like this
    // for_each(v.begin(), v.end(), test::print);

    for_each(v.begin(), v.end(), [](auto & t)
                                 {
                                     t.print();
                                 });

    for_each(v.begin(), v.end(), mem_fn(&test::print));
}
