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
        TypePtr type;
        StringView name;
        DeclarationExpression(TypePtr type, StringView name) :
            type(type), name(name) 
        {}

        ExpressionType GetType() const override { return DECLARATION; }
    };
}
}
