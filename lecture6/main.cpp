#include <iostream>
#include <memory>

#include "expression.hpp"
#include "literal.hpp"
#include "sum.hpp"
#include "reference.hpp"

using namespace std;

int main()
{
    auto c1 {make_shared<literal>(1)};
    auto c2 {make_shared<sum>(make_unique<reference>(c1), make_unique<literal>(2))};
    auto c3 {make_shared<literal>(10)};

    sum s {make_unique<reference>(c1), make_unique<literal>(2)};
    sum s2 {move(s)};

    cout << c2->getVal() << "\n";

    c1->setVal(2);
    cout << c2->getVal() << "\n";

    c2->setReferenceLeft(c3);
    cout << c2->getVal() << "\n";
}
