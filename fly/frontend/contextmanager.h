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
    IL::VariableExpressionPtr ContextManager::DoProcess(IL::VariableExpressionPtr val)
    {
        std::string errormessage;
        bool success = FillVariable(val->name, &(val->variable), &errormessage);
        if (!success)
            return IL::CreateError<IL::VariableExpression>(errormessage);

        return val;
    }

    template<>
    IL::DeclarationExpressionPtr ContextManager::DoProcess(IL::DeclarationExpressionPtr val)
    {
        IL::TypePtr type;
        std::string errormessage;
        bool success = FillType(val->type, &type, &errormessage);
        if (!success)
            return IL::CreateError<IL::DeclarationExpression>(errormessage);

        std::string errormessage2;
        success = AddVariable(val->name, type, &errormessage2);
        if (!success)
            return IL::CreateError<IL::DeclarationExpression>(errormessage2);

        return val;
    }

}
}


