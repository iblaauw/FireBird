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
    typedef std::map<std::string, std::vector<int>> LabelMissingCache;

    std::map<std::string, int> labels;
    LabelMissingCache labelsNeeded;
    LabelMissingCache dataLabelsNeeded;
public:
    std::vector<opvalue> ops;

    void Parse(const std::string& line);
    void Finalize();
private:
    std::string RemoveComments(const std::string& line);
    void ParseOp(const Tokens& tokens);
    void ParseConst(const std::string& val);
    void ParseFloatConst(const std::string& val);
    void HandleOpArgs(opvalue& val, const Tokens& tokens, unsigned int numArgs);
    void SetReg(const std::string& regval, opvalue& op, unsigned int index);
    void SetImm(const std::string& regval, opvalue& op);

    void AddLabel(const std::string& label);
    void SetLabelImm(const std::string& label, opvalue& val);
    void BackAddLabel(const std::string& label, LabelMissingCache& cache, uint16_t address, bool isDataLabel);

    void EnsureCacheEmpty(const LabelMissingCache& cache);
};

DECL_EXCEPTION(CompileException);

