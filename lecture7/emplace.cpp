#include <iostream>
#include <vector>

using namespace std;

class test
{
public:
	test() { cerr << "ctor\n"; }
	test(const test &) { cerr << "copy ctor\n"; }
	test(test &&) noexcept { cerr << "move ctor\n"; }
	~test() { cerr << "dtor\n"; }
};

int main()
{
    vector<test> v;

    for (int i = 0; i < 10; ++i)
    {
        // v.push_back(test {}); // unnecessary move constructor needed
		v.emplace_back();
    }
}
