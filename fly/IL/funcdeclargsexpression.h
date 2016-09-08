#pragma once

#include "expression.h"
#include "declarationexpression.h"

#include <vector>

namespace firefly
{
namespace IL
{
    class FuncDeclArgsExpression : public Expression
    {
    public:
        std::vector<DeclarationExpressionPtr> args;

        ExpressionType GetType() const override { return FUNC_DECL_ARGS; }
        void DebugPrint(TreePrinter& printer) const override
        {
            printer.Print('(');
            printer.PrintLine();
            printer.Increase();
            for (auto val : args)
            {
                val->DebugPrint(printer);
                printer.Print(',');
            }
            printer.PrintLine();
            printer.Decrease();
            printer.Print(')');
        }
    };

    using FuncDeclArgsExpressionPtr = std::shared_ptr<FuncDeclArgsExpression>;
}
}
