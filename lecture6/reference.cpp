#include "reference.hpp"

using namespace std;

reference::reference(weak_ptr<expression> l) : linked {l}
{}

void reference::setLink(weak_ptr<expression> l)
{
    linked = l;
}

int reference::getVal() const
{
    if (auto sp = linked.lock())
    {
        return sp->getVal();
    }

    // TODO
    throw 1;
}
