#pragma once

#include "expression.h"
#include "function.h"

#include <vector>

namespace firefly
{
namespace IL
{
    class FunctionCallExpression : public Expression
    {
    public:
        FunctionPtr function;
        std::vector<ExpressionPtr> arguments;

        ExpressionType GetType() const override { return FUNC_CALL; }
    };
}
}
