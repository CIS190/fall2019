#include <iostream>
#include <iterator>

using namespace std;

int main()
{
    ostream_iterator<string> oi {cout};

    *oi = "Hello ";
    oi++; // no-op
    *oi = "world"; // The * is also a no-op
}
