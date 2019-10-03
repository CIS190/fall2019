#include <iostream>
#include <memory>

#include "expression.hpp"
#include "literal.hpp"
#include "sum.hpp"

using namespace std;

int main()
{
    sum s {make_unique<literal>(1), make_unique<literal>(2)};

    cout << s.getVal();
}
