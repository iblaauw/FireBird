#pragma once

#include "common.h"
#include "message_exception.h"
#include "opvalue.h"
#include <map>
#include <string>
#include <vector>

class OpParser
{
private:
    static std::map<std::string, uint8_t> nameToOp;

    std::vector<opvalue> ops;
public:
    void Parse(const std::string& line);
};

class CompileException : public MessageException
{
public:
    CompileException(const char* m) : MessageException(m) {}
    CompileException(const std::string& m) : MessageException(m) {}
};
