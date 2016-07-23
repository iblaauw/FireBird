#pragma once

#include "intvals.h"

#include <map>
#include <string>

// A static store of info about compiler defined opcode names
class OpAttributes
{
public:
    typedef uint8_t OpCode;
    typedef uint8_t OpFlag;
private:
    struct AttributeSet
    {
        OpCode op;
        OpFlag flag;
        uint8_t numArgs;
    };

    static const std::map<std::string, AttributeSet> masterSet;
public:
    static bool TryGetAttributes(const std::string& token, OpCode* outOp, OpFlag* outFlag, uint8_t* numArgs);
};
