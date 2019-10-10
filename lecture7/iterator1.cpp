#include <iostream>
#include <list>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};

    for (list<int>::iterator i = l.begin(); i != l.end(); ++i)
        cout << *i << "\n";
}
