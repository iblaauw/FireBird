#pragma once

#include "expression.h"
#include "declarationexpression.h"

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
        std::vector<DeclarationExpressionPtr> args;
        std::vector<ExpressionPtr> expressions;

        ExpressionType GetType() const override { return FUNCTION; }
    };

    using FunctionExpressionPtr = std::shared_ptr<FunctionExpression>;
}
}

