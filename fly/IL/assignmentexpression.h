#pragma once

#include "expression.h"
#include "typedexpression.h"
#include "variableexpression.h"

namespace firefly
{
namespace IL
{
    class AssignmentExpression : public Expression
    {
    public:
        VariableExpressionPtr variable;
        TypedExpressionPtr value;

        ExpressionType GetType() const override { return ASSIGNMENT; }
        void DebugPrint(TreePrinter& printer) const override;
    };

    using AssignmentExpressionPtr = std::shared_ptr<AssignmentExpression>;
}
}
