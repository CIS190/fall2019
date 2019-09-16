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

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

int main()
{
    integer i {1};
    integer j {2};
    integer k {i};

    cout << k.get() << "\n";
}
