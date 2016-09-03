#include "contextmanager.h"

#include "frontendexception.h"

using namespace firefly::frontend;

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
    currentContext = IL::Context::TopLevel();
    InitBuiltins();
}

bool ContextManager::FillVariable(StringView name, IL::VariablePtr* outVariable, std::string* outError)
{
    bool success = currentContext->TryGetVariable(name, outVariable);
    if (!success)
    {
        *outError = "Unknown variable '" + name + "'.";
    }
    return success;
}

bool ContextManager::FillType(StringView name, IL::TypePtr* outType, std::string* outError)
{
    bool success = currentContext->TryGetType(name, outType);
    if (!success)
    {
        *outError = "Uknown type '" + name + "'.";
    }
}

bool ContextManager::AddVariable(StringView name, IL::TypePtr type, std::string* outError)
{
    if (currentContext->IsNameUsed(name))
    {
        *outError = "The name '"+ name +"' is already used.";
        return false;
    }

    IL::VariablePtr variable = std::make_shared<IL::Variable>();
    variable->name = name;
    variable->type = type;

    currentContext->AddVariable(variable);

    return true;
}

