#pragma once

#include "typedexpression.h"
#include "variable.h"

namespace firefly
{
namespace IL
{
    class VariableExpression : public TypedExpression
    {
    public:
        StringView variable;

        ExpressionType GetType() const override { return VARIABLE; }
        TypePtr GetILType() const override { return variable->type; }

    };
}
