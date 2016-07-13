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
    typedef std::vector<std::string> Tokens;

    static const std::map<std::string, uint8_t> nameToOp;
    static const std::map<uint8_t, unsigned int> opTypeMap;

    std::map<std::string, int> labels;
    std::map<std::string, std::vector<int>> labelsNeeded;
public:
    std::vector<opvalue> ops;

    void Parse(const std::string& line);
    void Finalize();
private:
    std::string RemoveComments(const std::string& line);
    void ParseOp(const Tokens& tokens);
    void HandleOpArgs(opvalue& val, const Tokens& tokens, unsigned int numArgs);
    void SetReg(const std::string& regval, opvalue& op, unsigned int index);
    void SetImm(const std::string& regval, opvalue& op);

    void AddLabel(const std::string& label);
    void SetLabelImm(const std::string& label, opvalue& val);
};

DECL_EXCEPTION(CompileException);

