#pragma once

#include "context.h"
#include "hiddenerror.h"

#include "variableexpression.h"
#include "declarationexpression.h"
#include "functionexpression.h"
#include "variable.h"
#include "type.h"

#include <StringView.h>
#include <string>
#include <memory>

namespace firefly
{
namespace frontend
{
    class ContextManager
    {
        IL::ContextPtr currentContext;
    public:
        ContextManager();

        inline IL::ContextPtr Current() { return currentContext; }

        template <class T>
        std::shared_ptr<T> Process(std::shared_ptr<T> val)
        {
            if (val->GetType() == IL::ERROR)
                return val;

            return DoProcess(val);
        }

        void EnterSubContext();
        void ExistSubContext();
    private:
        template <class T>
        std::shared_ptr<T> DoProcess(std::shared_ptr<T> val)
        {
            return val;
        }

        bool FillVariable(StringView name, IL::VariablePtr* outVariable, std::string* outError);
        bool FillType(StringView name, IL::TypePtr* outType, std::string* outError);

        bool AddVariable(StringView name, IL::TypePtr type, std::string* outError);
        bool AddFunction();
    };

    template <>
    IL::VariableExpressionPtr ContextManager::DoProcess(IL::VariableExpressionPtr);
    template <>
    IL::DeclarationExpressionPtr ContextManager::DoProcess(IL::DeclarationExpressionPtr);

}
}


