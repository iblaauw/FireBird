#pragma once

#include "expression.h"

#include <StringView.h>
#include <memory>
#include <vector>

namespace firefly
{
namespace IL
{
    class FunctionExpression : public Expression
    {
    public:
        StringView returnType;
        StringView name;
        std::vector<VariableExpressionPtr> args;
        std::vector<ExpressionPtr> expressions;

        ExpressionType GetType() const override { return FUNCTION; }
    };

    using FunctionExpressionPtr = std::shared_ptr<FunctionExpression>;
}
}

