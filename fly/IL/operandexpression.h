#pragma once

#include "expression.h"
#include "StringView.h"

namespace firefly
{
namespace IL
{
    class OperandExpression : public Expression
    {
    public:
        StringView operand;
        ExpressionPtr left, right;
        Type resultingType;

        OperandExpresion(StringView operand, 
            ExpressionPtr left, ExpressionPtr right) :
            operand(operand), left(left), right(right)
        {}

        ExpressionType GetType() override const { return OPERAND; }
    };
}
}

