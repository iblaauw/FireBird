#include "context.h"
#include "ilexception.h"
#include "function.h"

using Context = firefly::IL::Context;
using ContextPtr = firefly::IL::ContextPtr;

/*static*/ContextPtr Context::topInstance(new Context());

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

bool Context::TryGetType(StringView name, TypePtr* outVal)
{
    bool success = TryGetFromMap(name, types, outVal);
    if (success)
        return true;

    if (parent == nullptr)
        return false;

    return parent->TryGetType(name, outVal);
}

void Context::AddVariable(VariablePtr variable)
{
    StringView name = variable->name;
    std::cout << "*** Adding variable: " << name << std::endl;
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

void Context::AddType(TypePtr type)
{
    StringView name = type->value;
    if (IsNameUsed(name))
        throw ILException("Cannot declare function, the name '" + static_cast<std::string>(name) + "' is already being used.");

    types.emplace(name, type);
}

ContextPtr Context::CreateChild()
{
    Context* newContext = new Context(this);
    ContextPtr ptr(newContext);
    children.push_back(ptr);
    return ptr;
}

bool Context::IsNameUsed(StringView name)
{
    bool var,func,type;
    TypeOfName(name, &var, &func, &type);
    return var || func || type;
}

void Context::TypeOfName(StringView name, bool* variable, bool* func, bool* type)
{
    *variable = *func = *type = false;

    auto iter = variables.find(name);
    if (iter != variables.end())
    {
        *variable = true;
        return;
    }

    auto iter2 = functions.find(name);
    if (iter2 != functions.end())
    {
        *func = true;
        return;
    }

    auto iter3 = types.find(name);
    if (iter3 != types.end())
    {
        *type = true;
        return;
    }

    if (parent == nullptr)
        return;

    return parent->TypeOfName(name, variable, func, type);
}


