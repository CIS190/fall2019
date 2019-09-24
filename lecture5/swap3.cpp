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

    integer & operator=(integer i)
    {
        using std::swap; // not needed since we already did using namespace std; above. If we didn't, we would need this for the fallback to std::swap to work.
        swap(*this, i); // since the arguments are in the integer namespace, this swap chooses integer::swap

        cout << "=\n";
        return *this;
    }

    friend void swap(integer & i, integer & j) noexcept
    {
        using std::swap;
        swap(i.p, j.p);
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
