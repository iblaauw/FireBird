#pragma once

#include "expression.h"

namespace firefly
{
namespace IL
{
    class IdentifierExpression : public Expression
    {
    public:
        StringView name;

        ExpressionType GetType() const override { return IDENTIFIER; }
        void DebugPrint(TreePrinter& printer) const override { printer.Print(name); }
    };

    using IdentifierExpressionPtr = std::shared_ptr<IdentifierExpression>;
}
}

