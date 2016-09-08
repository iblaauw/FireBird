#pragma once

#include "type.h"
#include "typedexpression.h"

namespace firefly
{
namespace IL
{
    class TypeExpression : public TypedExpression
    {
    public:
        TypePtr type;

        ExpressionType GetType() const override { return TYPE; }
        void DebugPrint(TreePrinter& printer) const override { printer.Print(type->value); }
        TypePtr GetILType() const override { return type; }
    };

    using TypeExpressionPtr = std::shared_ptr<TypeExpression>;
}
}
