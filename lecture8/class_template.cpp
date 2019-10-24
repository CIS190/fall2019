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

using namespace std;

int main()
{
    wrapper w {12};

    cout << w.get();
}
