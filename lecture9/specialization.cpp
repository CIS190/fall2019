#include <iostream>

template <typename T>
class wrapper
{
    T t;
public:
    wrapper(T t) : t {t} {}

    T get() const
    {
        return t;
    }

    void set(T t)
    {
        this.t = t;
    }
};

// Custom behavior for ints only where we ignore other operations and only return 1
template <>
class wrapper<int>
{
public:
    wrapper(int) {}

    int get() const
    {
        return 1;
    }

    void set(int) {}
};

using namespace std;

int main()
{
    wrapper w {12};

    cout << w.get();
}
