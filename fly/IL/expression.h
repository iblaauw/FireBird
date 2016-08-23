#pragma once

#include <StringView>
#include <memory>

namespace firefly
{
namespace IL
{
    enum ExpressionType
    {
        ERROR = -1,
        OPERAND,
        DEFINITION,
        ASSIGNMENT,
        FUNC_CALL,
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

        ExpressionType GetType() override const { return ERROR; }
    };

    using ExpressionPtr = std::shared_ptr<Expression>;

    void AssertNonNull(ExpressionPtr ptr);
}
}
