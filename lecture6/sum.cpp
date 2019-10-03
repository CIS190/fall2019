#include "sum.hpp"

using namespace std;

sum::sum(unique_ptr<expression> l, unique_ptr<expression> r) : left {move(l)}, right {move(r)}
{}

int sum::getVal() const
{
    // ignoring null case
    return left->getVal() + right->getVal();
}
