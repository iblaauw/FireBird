#include "contextmanager.h"

#include "frontendexception.h"

using namespace firefly;
using ContextManager = firefly::frontend::ContextManager;

static void InitBuiltins()
{
    IL::TypePtr intType = std::make_shared<IL::Type>("int");
    IL::TypePtr voidType = std::make_shared<IL::Type>("void");

    IL::ContextPtr context = IL::Context::GetTopLevel();
    context->AddType(intType);
    context->AddType(voidType);
}

ContextManager::ContextManager()
{
    currentContext = IL::Context::GetTopLevel();
    InitBuiltins();
}

bool ContextManager::FillVariable(StringView name, IL::VariablePtr* outVariable, std::string* outError)
{
    bool success = currentContext->TryGetVariable(name, outVariable);
    if (!success)
    {
        *outError = "Unknown variable '" + static_cast<std::string>(name) + "'.";
    }
    return success;
}

bool ContextManager::FillType(StringView name, IL::TypePtr* outType, std::string* outError)
{
    bool success = currentContext->TryGetType(name, outType);
    if (!success)
    {
        *outError = "Uknown type '" + static_cast<std::string>(name) + "'.";
    }
    return success;
}

bool ContextManager::AddVariable(StringView name, IL::TypePtr type, std::string* outError)
{
    if (currentContext->IsNameUsed(name))
    {
        *outError = "The name '"+ static_cast<std::string>(name) +"' is already used.";
        return false;
    }

    IL::VariablePtr variable = std::make_shared<IL::Variable>();
    variable->name = name;
    variable->type = type;

    currentContext->AddVariable(variable);

    return true;
}

//template <class T>
//std::shared_ptr<T> ContextManager::DoProcess(std::shared_ptr<T> val)
//{
//    return val;
//}

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

