#pragma once

#include "expression.h"
#include "typeexpression.h"

namespace firefly
{
namespace IL
{
    class DeclarationExpression : public Expression
    {
    public:
        TypeExpressionPtr type;
        StringView name;
        VariablePtr variable;

        ExpressionType GetType() const override { return DECLARATION; }
        void DebugPrint(TreePrinter& printer) const override;
    };

    using DeclarationExpressionPtr = std::shared_ptr<DeclarationExpression>;
}
}
