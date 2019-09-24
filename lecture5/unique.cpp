#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

class test
{
public:
    ~test()
    {
        cerr << "~test\n";
    }
};

unique_ptr<test> f(unique_ptr<test> p)
{
    return p;
}

int main()
{
    auto p {make_unique<test>()};
    auto q {make_unique<test>()};

    // p = q; // doesn't work
    p = move(q);

    // p = f(q); // doesn't work
    p = f(move(q));

    assert(!q); // q is invalidated after the move
}
