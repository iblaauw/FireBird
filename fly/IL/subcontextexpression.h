#pragma once

#include "expression.h"
#include "context.h"

#include <vector>

namespace firefly
{
namespace IL
{
    class SubContextExpression : public Expression
    {
    public:
        std::vector<ExpressionPtr> expressions;
        ContextPtr context;

        ExpressionType GetType() const override { return SUBCONTEXT; }
        void DebugPrint(TreePrinter& printer) const override
        {
            printer.Print('{');
            printer.PrintLine();
            printer.Increase();
            for (auto val : expressions)
            {
                val->DebugPrint(printer);
                printer.PrintLine();
            }
            printer.Decrease();
            printer.Print('}');
            printer.PrintLine();
        }
    };

    using SubContextExpressionPtr = std::shared_ptr<SubContextExpression>;
}
}


