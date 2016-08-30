#pragma once

#include "typedexpression.h"
#include "variable.h"

namespace firefly
{
namespace IL
{
    class VariableExpression : public TypedExpression
    {
    public:
        StringView variable;
        TypePtr type;

        ExpressionType GetType() const override { return VARIABLE; }
        TypePtr GetILType() const override { return type; }
        void DebugPrint(TreePrinter& printer) const override { printer.Print(variable); }

    };

    using VariableExpressionPtr = std::shared_ptr<VariableExpression>;
}
}

