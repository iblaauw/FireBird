#pragma once

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
        CONSTANT
    };

    class Expression
    {
        virtual ExpressionType GetType() const = 0;
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
    };

    using ExpressionPtr = std::shared_ptr<Expression>;

    void AssertNonNull(ExpressionPtr ptr);
}
}

