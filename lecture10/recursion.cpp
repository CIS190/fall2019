#include <iostream>
#include <functional>

using namespace std;


int main()
{
    function<int(int)> fib = [&fib](int i)
               {
                   if (i <= 1)
                       return 1;
                   return fib(i - 1) + fib(i - 2);
               };

    cout << fib(10);
}
