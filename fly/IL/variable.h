#pragma once

#include "type.h"
#include <StringView.h>

namespace firefly
{
namespace IL
{
    class Variable
    {
    public:
        TypePtr type;
        StringView name;
    };

    using VariablePtr = std::shared_ptr<Variable>;
}
}
