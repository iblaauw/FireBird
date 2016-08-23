#pragma once

#include "expression.h"
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

        Function(const std::string& name) : name(name), expressions()
        {}

        void AddExpression(ExpressionPtr exp); 
        iter_type begin();
        iter_type end();
    };
}
}
