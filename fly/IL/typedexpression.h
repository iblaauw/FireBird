#pragma once

#include "expression.h"
#include "type.h"

namespace firefly
{
namespace IL
{
    class TypedExpression : public Expression
    {
    public:
        TypePtr GetILType() const = 0;
    };
}
}
