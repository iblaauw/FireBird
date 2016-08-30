#pragma once

#include "expression.h"
#inlucde "type.h"

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
    };
}
}
