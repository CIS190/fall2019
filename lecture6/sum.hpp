#ifndef SUM_HPP
#define SUM_HPP

#include <memory>

#include "expression.hpp"

class sum : public expression
{
    std::unique_ptr<expression> left;
    std::unique_ptr<expression> right;

    expression left;
public:

    sum(std::unique_ptr<expression>, std::unique_ptr<expression>);

    int getVal() const;

    void setReferenceLeft(std::weak_ptr<expression>);
    void setReferenceRight(std::weak_ptr<expression>);
};

#endif
