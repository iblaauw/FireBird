#pragma once

#include "expression.h"
#include "typedexpression.h"

namespace firefly
{
namespace IL
{
    class AssignmentExpression : public Expression
    {
    public:
        VariableExpressionPtr variable;
        TypedExpression value;

        ExpressionType GetType() const override { return ASSIGNMENT; }
    };
}
}
