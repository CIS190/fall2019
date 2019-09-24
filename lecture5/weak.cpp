#include <iostream>
#include <memory>

using namespace std;

class test
{
public:
	// shared_ptr<test> other; // never destroyed if there is a cycle
	weak_ptr<test> other;

	void foo()
	{
		if (auto shared = other.lock())
		{
			// do something with the shared_ptr to other
			cerr << "obtained shared_ptr to other\n";
		}
	}

    ~test()
    {
        cerr << "~test\n";
    }
};

int main()
{
	{
		auto t1 {make_shared<test>()};
		auto t2 {make_shared<test>()};

		t1->other = t2;
		t2->other = t1;

		t1->foo();
	}

	cerr << "shared_ptrs both out of scope\n";
}
