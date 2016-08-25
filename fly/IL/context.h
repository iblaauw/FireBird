#pragma once

#include "expression.h"
#include "variable.h"
#include "type.h"
#include <StringView.h>

#include <map>

namespace firefly
{
namespace IL
{
    class Context;

    using ContextPtr = std::shared_ptr<Context>;

    class Function;
    using FunctionPtr = std::shared_ptr<Function>;

    class Context
    {
    private:
        static Context* topInstance;

        Context* parent;

        std::map<StringView,VariablePtr> variables;
        std::map<StringView,FunctionPtr> functions;
        std::map<StringView,TypePtr> types;

        Context() : parent(nullptr) {}
        Context(Context* parent) : parent(parent) {}
    public:

        static Context* GetTopLevel() { return topInstance; }

        bool TryGetVariable(StringView name, VariablePtr* outVal);
        bool TryGetFunction(StringView name, FunctionPtr* outVal);
        bool TryGetType(StringView name, TypePtr* outVal);

        void AddVariable(VariablePtr variable);
        void AddFunction(FunctionPtr function);
        void AddType(TypePtr type);

        ContextPtr CreateChild();

        bool IsNameUsed(StringView name);
    };

}
}

