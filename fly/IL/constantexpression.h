#pragma once

#include "typedexpression.h"
#include <StringView.h>

namespace firefly
{
namespace IL
{
    class ConstantExpression : public TypedExpression
    {
    public:
        StringView constant;
        TypePtr type;

        ExpressionType GetType() const override { return CONSTANT; }
        TypePtr GetILType() const override { return type; }
    };

    using ConstantExpressionPtr = std::shared_ptr<ConstantExpression>;
}
}
