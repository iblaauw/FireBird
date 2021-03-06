#pragma once

#include "expression.h"
#include "functionsignature.h"
#include "context.h"

#include <string>
#include <vector>

namespace firefly
{
namespace IL
{
    class Function
    {
    private:
        StringView name;
        std::vector<ExpressionPtr> expressions;
    public:
        typedef decltype(expressions.begin()) iter_type;

        FunctionSignaturePtr signature;
        ContextPtr functionContext;

        Function(FunctionSignaturePtr sig, ContextPtr funcContext);

        void AddExpression(ExpressionPtr exp);
        iter_type begin();
        iter_type end();

        inline StringView GetName() const { return name; }
    };

    using FunctionPtr = std::shared_ptr<Function>;
}
}
