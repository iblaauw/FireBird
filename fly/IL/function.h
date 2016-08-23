#pragma once

#include "expression.h"
#include "functionsignature.h"

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

        Function(const std::string& name, FunctionSignaturePtr sig) 
            : name(name), expressions(), signature(sig)
        {}

        void AddExpression(ExpressionPtr exp);
        iter_type begin();
        iter_type end();
    };

    using FUnctionPtr = std::shared_ptr<Function>;
}
}
