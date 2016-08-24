#include "context.h"
#include "ilexception.h"
#include "function.h"

using Context = firefly::IL::Context;
using ContextPtr = firefly::IL::ContextPtr;

/*static*/Context* Context::topInstance = new Context();

template <class PtrType>
bool TryGetFromMap(StringView key, const std::map<StringView,PtrType>& map, PtrType* outVal)
{
    auto iter = map.find(key);
    if (iter != map.end())
    {
        *outVal = iter->second;
        return true;
    }
    return false;
}

bool Context::TryGetVariable(StringView name, VariablePtr* outVal)
{
    bool success = TryGetFromMap(name, variables, outVal);
    if (success)
        return true;

    if (parent == nullptr)
        return false;

    return parent->TryGetVariable(name, outVal);
}

bool Context::TryGetFunction(StringView name, FunctionPtr* outVal)
{
    bool success = TryGetFromMap(name, functions, outVal);
    if (success)
        return true;

    if (parent == nullptr)
        return false;

    return parent->TryGetFunction(name, outVal);
}

bool TryGetType(StringView name, TypePtr* outVal)
{
    bool success = TryGetFromMap(name, types, outVal);
    if (success)
        return true;

    if (parent == nullptr)
        return false;

    return parent->TryGetTypes(name, outVal);
}

void Context::AddVariable(VariablePtr variable)
{
    StringView name = variable->name;
    if (IsNameUsed(name))
        throw ILException("Cannot declare variable, the name '" + static_cast<std::string>(name) + "' is already being used.");

    variables.emplace(name, variable);
}

void Context::AddFunction(FunctionPtr function)
{
    StringView name = function->GetName();
    if (IsNameUsed(name))
        throw ILException("Cannot declare function, the name '" + static_cast<std::string>(name) + "' is already being used.");

    functions.emplace(name, function);
}

ContextPtr Context::CreateChild() 
{ 
    Context* newcontext = new Context(this);
    return ContextPtr(newcontext);
}

bool Context::IsNameUsed(StringView name)
{
    auto iter = variables.find(name);
    if (iter != variables.end())
        return true;

    auto iter2 = functions.find(name);
    if (iter2 != functions.end())
        return true;

    if (parent == nullptr)
        return false;

    return parent->IsNameUsed(name);
}


