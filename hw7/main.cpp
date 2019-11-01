#include <iostream>

using namespace std;

template<typename F>
void print(F f)
{
    cout << "Calling f: ";
    f();
    cout << "\n";
}

int main()
{
    int i {0};

    // Create your lambda expression
    cout << "Lambda: \n";
    for (i = 0; i < 10; ++i)
    {
        // Call print with the lambda
    }

    // Create your functor
    cout << "Functor: \n";
    for (i = 0; i < 10; ++i)
    {
        // Call print with the functor
    }
}
