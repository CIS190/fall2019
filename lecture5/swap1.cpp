#include <iostream>
#include <utility>

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

    integer(const integer & i) : p {new int {i.get()}}
    {
        cout << "copy ctor\n";
    }
    integer(integer && i) : p {i.p}
    {
        i.p = nullptr;
        cout << "move ctor\n";
    }
    ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(const integer & i)
    {
        integer temp {i}; // copy

        swap(p, temp.p);

        cout << "copy=\n";
        return *this;
    }
    integer & operator=(integer && i)
    {
        integer temp {move(i)}; // move

        swap(p, temp.p);

        cout << "move=\n";
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
}

int main()
{
    integer i {1};
    integer j {2};

    j = incr(i);

    cout << j.get() << "\n";
}
