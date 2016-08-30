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

    class OperandExpression;

    using OperandExpressionPtr = std::shared_ptr<OperandExpression>;

    class OperandExpression : public TypedExpression
    {
    public:
        OperandType type;
        TypedExpressionPtr left, right;
        TypePtr resultingType;

        static OperandExpressionPtr Error(const std::string& message);

        ExpressionType GetType() const override { return OPERAND; }
        TypePtr GetILType() const override { return resultingType; }
    };
}
}

