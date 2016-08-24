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
        std::string name;
        std::vector<ExpressionPtr> expressions;
    public:
        typedef decltype(expressions.begin()) iter_type;

        FunctionSignaturePtr signature;
        ContextPtr functionContext;

        Function(const std::string& name, FunctionSignaturePtr sig, ContextPtr currentContext);

        void AddExpression(ExpressionPtr exp);
        iter_type begin();
        iter_type end();

        inline const std::string& GetName() const { return name; }
    };

    using FUnctionPtr = std::shared_ptr<Function>;
}
}
