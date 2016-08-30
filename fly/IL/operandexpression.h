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

        ExpressionType GetType() const override { return OPERAND; }
        TypePtr GetILType() const override { return resultingType; }
        void DebugPrint(TreePrinter& printer) const override;
    };

    using OperandExpressionPtr = std::shared_ptr<OperandExpression>;
}
}

