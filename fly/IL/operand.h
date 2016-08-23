#pragma once

#include "expression.h"

namespace firefly
{
namespace IL
{
    class OperandExpression : public Expression
    {
    public:
        StringView operand;
        Expression* left, right;

        OperandExpresion(StringView operand, 
            Expression* left, Expression* right) :
            operand(operand), left(left), right(right)
        {}

        ExpressionType GetType() override const { return OPERAND; }
    };
}
}

