#include "literal.hpp"

literal::literal(int v) : val {v}
{ }

int literal::getVal() const
{
    return val;
}
