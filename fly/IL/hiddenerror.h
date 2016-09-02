#pragma once

#include "expression.h"
#include "ilexception.h"

#include <StringView.h>
#include <memory>

namespace firefly
{
namespace IL
{
    template <class T>
    class HiddenErrorExpression : public T
    {
    public:
        std::string message;
        ExpressionType GetType() const override { return ERROR; }
        void DebugPrint(TreePrinter& printer) const override
        {
            printer.Print("[ERROR: ");
            printer.Print(message);
            printer.Print(']');
        }
    };

    template <class T>
    std::shared_ptr<T> CreateError(const std::string& message)
    {
        auto error = std::make_shared<HiddenErrorExpression<T>>();
        error->message = message;
        return std::static_pointer_cast<T>(error);
    }

    template <class T>
    std::string GetErrorMessage(std::shared_ptr<T> error)
    {
        if (error->GetType() != ERROR)
            throw ILException("Cannot get error message from non-error.");

        auto hiddenError = std::dynamic_pointer_cast<HiddenErrorExpression<T>>(error);
        if (hiddenError == nullptr)
            throw ILException("Cannot get error message. Error is not a HiddenErrorExpression.");

        return hiddenError->message;
    }

    template <class T, class U>
    std::shared_ptr<T> RecastError(std::shared_ptr<U> error)
    {
        std::string message = GetErrorMessage(error);
        return CreateError<T>(message);
    }
}
}
