#pragma once

#include "expression.h"
#include "type.h"
#include <StringView.h>

namespace firefly
{
namespace IL
{
    class ConstantExpression : public Expression
    {
    public:
        TypePtr type;
        StringView constant;

        ExpressionType GetType() const override { return CONSTANT; }
    };
}
}
