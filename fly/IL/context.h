#pragma once

#include "expression.h"
#include "variable.h"
#include "type.h"
#include <StringView.h>

#include <map>
#include <vector>

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
        static ContextPtr topInstance;

        Context* parent;

        std::map<StringView,VariablePtr> variables;
        std::map<StringView,FunctionPtr> functions;
        std::map<StringView,TypePtr> types;

        std::vector<ContextPtr> children;

        Context() : parent(nullptr) {}
        Context(Context* parent) : parent(parent) {}
    public:

        static ContextPtr GetTopLevel() { return topInstance; }

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

