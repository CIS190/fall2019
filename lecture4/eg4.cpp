#include <iostream>

using namespace std;

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        cout << "ctor\n";
    }

    // could also do : integer(i.get()), which would be better since it repeats less code, but it would also print more than I want
    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(const integer & i)
    {
        *p = i.get();
        cout << "copy=\n";
        return *this;
    }

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

integer incr(integer i)
{
    i.set(i.get() + 1);
    return i;

    // In some cases, C++ is a bit smarter and can omit copies and call the constructor "outside" the function, like the following.
    // Note that this isn't a compiler-specific optimization, the specification actually requires this.
    // So don't rely on side effects in your copy constructor! Copy construction from a freshly constructed object should be equivalent to using the freshly constructed object itself.
    // return integer {i.get() + 1};
}

int main()
{
    integer i {1};
    integer j {2};
    j = i;

    cout << j.get() << "\n";
}
