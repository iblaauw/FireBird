#pragma once

#include "treeprinter.h"

#include <StringView.h>
#include <memory>

namespace firefly
{
namespace IL
{
    enum ExpressionType
    {
        ERROR = -1,
        OPERAND,
        DECLARATION,
        ASSIGNMENT,
        FUNC_CALL,
        VARIABLE,
        CONSTANT,
        FUNCTION,
        TYPE,
        IDENTIFIER,
        SUBCONTEXT,
        FUNC_DECL_ARGS,
        FUNC_DECL,
    };

    class Expression
    {
    public:
        virtual ExpressionType GetType() const = 0;
        virtual void DebugPrint(TreePrinter& printer) const = 0;
    };

    using ExpressionPtr = std::shared_ptr<Expression>;

    void AssertNonNull(ExpressionPtr ptr);
}
}

