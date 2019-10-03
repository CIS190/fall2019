#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include <memory>

#include "expression.hpp"

class reference : public expression
{
private:
    std::weak_ptr<expression> linked;
public:
    reference() = default;
    reference(std::weak_ptr<expression>);

    void setLink(std::weak_ptr<expression>);
    int getVal() const;
};

#endif
