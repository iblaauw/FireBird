#pragma once

#include "typedexpression.h"
#include "function.h"

#include <vector>

namespace firefly
{
namespace IL
{
    class FunctionCallExpression : public TypedExpression
    {
    public:
        FunctionPtr function;
        std::vector<TypedExpressionPtr> arguments;

        ExpressionType GetType() const override { return FUNC_CALL; }
        TypePtr GetILType() const override { return function->signature->returnType; }
    };
}
}
