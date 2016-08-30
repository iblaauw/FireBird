#pragma once

#include "typedexpression.h"

namespace firefly
{
namespace IL
{
    enum OperandType
    {
        PLUS,
        MINUS,
        TIMES,
        DIVIDES,
    };

    class OperandExpression : public TypedExpression
    {
    public:
        OperandType type;
        TypedExpressionPtr left, right;
        TypePtr resultingType;

        ExpressionType GetType() override const { return OPERAND; }
        TypePtr GetILType() override const ( return resultingType; }
    };
}
}

