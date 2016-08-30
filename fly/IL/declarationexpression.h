#pragma once

#include "expression.h"
#include "type.h"

namespace firefly
{
namespace IL
{
    class DeclarationExpression : public Expression
    {
    public:
        StringView type;
        StringView name;

        ExpressionType GetType() const override { return DECLARATION; }
        void DebugPrint(TreePrinter& printer) const override;
    };

    using DeclarationExpressionPtr = std::shared_ptr<DeclarationExpression>;
}
}
