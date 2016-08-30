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
        virtual TypePtr GetILType() const = 0;
    };

    using TypedExpressionPtr = std::shared_ptr<TypedExpression>;
}
}
