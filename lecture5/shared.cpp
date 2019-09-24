#include <iostream>
#include <memory>

using namespace std;

class test
{
public:
	void foo()
	{
		cerr << "foo\n";
	}

    ~test()
    {
        cerr << "~test\n";
    }
};

int main()
{
    auto t1 {make_shared<test>()};
	auto t2 {t1};

	t1.reset();

	t2->foo();

	t2.reset();

	cerr << "shared_ptrs both cleared\n";
}
