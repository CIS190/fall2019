#include <iostream>

using namespace std;

void doSomething(int a, int i = 10)
{
    // ...
}

// note that you can no longer define the function
// void doSomething(int a)
// {
//     // ...
// }

int main()
{
    doSomething(10);
}
