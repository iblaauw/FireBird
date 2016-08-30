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
        FUNCTION
    };

    class Expression
    {
    public:
        virtual ExpressionType GetType() const = 0;
        virtual void DebugPrint(TreePrinter& printer) const = 0;
    };

    class ErrorExpression : public Expression
    {
    private:
        std::string message;
    public:
        ErrorExpression(const std::string& message) : message(message) {}
        ErrorExpression(const char* message) : message(message) {}
        ErrorExpression(const StringView& sv)
        {
            message = (std::string)sv;
        }

        ExpressionType GetType() const override { return ERROR; }
        void DebugPrint(TreePrinter& printer) const override { printer.Print(message); }
    };

    using ExpressionPtr = std::shared_ptr<Expression>;

    void AssertNonNull(ExpressionPtr ptr);
}
}

