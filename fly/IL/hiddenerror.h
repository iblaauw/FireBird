#pragma once

#include "expression.h"

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
        void DebugPrint(TreePrinter& printer) const override { printer.Print(message); }
    };

    template <class T>
    std::shared_ptr<T> CreateError(const std::string& message)
    {
        auto error = std::make_shared<HiddenErrorExpression<T>>();
        error->message = message;
        return std::static_pointer_cast<T>(error);
    }
}
}
