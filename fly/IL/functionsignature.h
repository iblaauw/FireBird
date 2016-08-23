#pragma once

#include "Type.h"

#include <StringView.h>
#include <memory>
#include <vector>

namespace firefly
{
namespace IL
{
    class FunctionSignature
    {
    public:
        StringView name;
        TypePtr returnType;
        std::vector<TypePtr> argumentTypes;
    };

    using FunctionSignaturePtr = std::shared_ptr<FunctionSignature>;
}
}
