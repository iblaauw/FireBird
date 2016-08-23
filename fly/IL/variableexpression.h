#pragma once

#include "expression.h"
#include "variable.h"

namespace firefly
{
namespace IL
{
    class VariableExpression : public Expression
    {
    public:
        VariablePtr variable;
        ExpressionType GetType() const override { return VARIABLE; }
    };
}
