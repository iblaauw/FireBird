#pragma once

#include "expression.h"
#include "funcdeclargsexpression.h"
#include "subcontextexpression.h"
#include "context.h"

namespace firefly
{
namespace IL
{
    class FunctionDeclarationExpression : public Expression
    {
    public:
        DeclarationExpressionPtr declaration;
        FuncDeclArgsExpressionPtr args;
        SubContextExpressionPtr subexpressions;

        ContextPtr context;

        ExpressionType GetType() const override { return FUNC_DECL; }
        void DebugPrint(TreePrinter& printer) const override
        {
            declaration->DebugPrint(printer);
            args->DebugPrint(printer);
            subexpressions->DebugPrint(printer);
        }
    };

    using FunctionDeclarationExpressionPtr = std::shared_ptr<FunctionDeclarationExpression>;
}
}
