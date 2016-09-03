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
        StringView name;
        VariablePtr variable;

        ExpressionType GetType() const override { return VARIABLE; }
        TypePtr GetILType() const override
        {
            if (variable == nullptr) throw ILException("FATAL: variable hasn't been context processed.");
            return variable->type; 
        }
        void DebugPrint(TreePrinter& printer) const override { printer.Print(variable); }

    };

    using VariableExpressionPtr = std::shared_ptr<VariableExpression>;
}
}

