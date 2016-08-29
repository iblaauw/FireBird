#pragma once

#include <StringView.h>
#include <memory>

namespace firefly
{
namespace IL
{
    class Type
    {
    public:
        StringView value;
        Type(StringView val): value(val) {}
        // This will become more complicated
    };

    using TypePtr = std::shared_ptr<Type>;
}
}
