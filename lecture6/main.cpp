#include <iostream>

#include "expression.hpp"
#include "literal.hpp"

using namespace std;

int main()
{
    literal l {1};

    cout << l.getVal();
}
