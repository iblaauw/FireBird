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
        Type(StringView val) : value(val), numBytes(4) {}

        // This will become more complicated
        int numBytes;
    };

    using TypePtr = std::shared_ptr<Type>;
}
}
