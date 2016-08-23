#pragma once

#include "typedexpression.h"
#include <StringView.h>

namespace firefly
{
namespace IL
{
    class ConstantExpression : public Expression
    {
    private:
        TypePtr type;
    public:
        StringView constant;

        ExpressionType GetType() const override { return CONSTANT; }
        TypePtr GetILType() const override { return type; }
    };
}
}
