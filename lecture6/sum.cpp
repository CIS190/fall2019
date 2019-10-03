#include "sum.hpp"
#include "reference.hpp"

using namespace std;

sum::sum(unique_ptr<expression> l, unique_ptr<expression> r) : left {move(l)}, right {move(r)}
{}

int sum::getVal() const
{
    // ignoring null case
    return left->getVal() + right->getVal();
}

void sum::setReferenceLeft(weak_ptr<expression> wp)
{
    if (reference * l = dynamic_cast<reference*>(left.get()))
    {
        l->setLink(wp);
    }
}

void sum::setReferenceRight(weak_ptr<expression> wp)
{
    if (reference * r = dynamic_cast<reference*>(right.get()))
    {
        r->setLink(wp);
    }
}
