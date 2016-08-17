#include "opparser.h"
#include "common.h"
#include "opcodes.h"
#include "split.h"
#include "stringutils.h"
#include "intvals.h"
#include "op_attributes.h"
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

#define COMMENT_CHAR '#'
#define LABEL_CHAR ':'
#define REG_CHAR 'r'


static inline bool IsRegister(const std::string& token)
{
    if (token.size() == 0)
        return false;

    return token[0] == REG_CHAR;
}

static inline bool IsLabel(const std::string& token)
{
    if (token.size() == 0)
        return false;

    return token[0] == LABEL_CHAR;
}

static inline bool IsConst(const std::string& token)
{
    return token == "const";
}

static inline bool IsHexVal(const std::string& token)
{
    return token.size() > 2 && token[0] == '0' && token[1] == 'x';
}

static inline uint16_t GetShortValue(const std::string& str)
{
    try
    {
        if (utils::IsHexStr(str))
        {
            return utils::ParseHexUint16(str);
        }

        return utils::ParseUint16(str);
    }
    catch (utils::StringParseException e)
    {
        std::string message = "Invalid constant '" + str + "'. Reason: " + e.what();
        throw CompileException(message);
    }
}

static inline uint32_t GetFullValue(const std::string& str)
{
    try
    {
        if (utils::IsHexStr(str))
        {
            return utils::ParseHexUint32(str);
        }

        return utils::ParseUint32(str);
    }
    catch (utils::StringParseException e)
    {
        std::string message = "Invalid constant '" + str + "'. Reason: " + e.what();
        throw CompileException(message);
    }
}


void OpParser::Parse(const std::string& line)
{
    std::string pure_line = RemoveComments(line);

    Tokens tokens = patch::split(pure_line, ' ');
    if (tokens.size() == 0) // Empty line
        return;

    if (IsLabel(tokens[0]))
    {
        std::cout << "Detected label " << tokens[0] << std::endl;

        if (tokens.size() != 1)
            throw CompileException(
                "Error: a label must be on its own line.");

        AddLabel(tokens[0]);
        return;
    }

    if (IsConst(tokens[0]))
    {
        if (tokens.size() != 2)
            throw CompileException("Error: const expressions has wrong number of arguments");

        auto& cToken = tokens[1];
        std::cout << "Detected const " << cToken << std::endl;

        ParseConst(cToken);

        return;
    }

    ParseOp(tokens);
}

void OpParser::Finalize()
{
    if (labelsNeeded.size() == 0)
        return;

    auto iter = labelsNeeded.begin();
    const auto& vec = iter->second;
    if (vec.size() == 0)
        return;

    std::string label = iter->first;
    std::string line = patch::to_string(vec[0]);

    throw CompileException("Error: unknown label '" + label + "', on line " + line);

}

std::string OpParser::RemoveComments(const std::string& line)
{
    Tokens tokens = utils::Split(line, COMMENT_CHAR, true);
    // Either an entire line of '#'s, or an empty line
    if (tokens.size() == 0)
        return "";

    return tokens[0];
}

void OpParser::ParseOp(const Tokens& tokens)
{
    assert(tokens.size() > 0);

    std::string opcode_name = tokens[0];

    uint8_t opcode;
    uint8_t opflag;
    uint8_t numArgs;
    bool success = OpAttributes::TryGetAttributes(
        opcode_name, &opcode, &opflag, &numArgs);

    if (!success)
        throw CompileException("Error: unknown opcode '" + opcode_name + "'");

    opvalue op;
    memset(&op, 0, sizeof(opvalue));
    op.op = opcode;
    op.opFlag = opflag;

    std::cout << opcode_name << std::endl;

    HandleOpArgs(op, tokens, numArgs);

    ops.push_back(op);
}

void OpParser::ParseConst(const std::string& val)
{
    assert(val.size() > 0);

    uint32_t ival = GetFullValue(val);
//    try
//    {
//        ival = utils::ParseUnsignedInt(val);
//    }
//    catch (...)
//    {
//        throw CompileException("Error: invalid constant");
//    }

    static_assert(sizeof(uint32_t) == sizeof(opvalue), "opvalue is the wrong size");

    opvalue* fakeop = reinterpret_cast<opvalue*>(&ival);
    ops.push_back(*fakeop);
}


void OpParser::HandleOpArgs(opvalue& val, const Tokens& tokens, unsigned int numArgs)
{
    assert(numArgs >= 0 && numArgs <= 3);

    if (tokens.size() != numArgs + 1)
        throw CompileException("Wrong number of arguments for opcode '" + tokens[0] + "'");

    if (numArgs == 0)
        return;

    for (unsigned int i = 1; i < tokens.size(); i++)
    {
        bool isLast = i == numArgs;
        const std::string& token = tokens[i];

        if (isLast && !IsRegister(token))
        {
            val.argFlag = 1;

            if (IsLabel(token))
            {
                SetLabelImm(token, val);
            }
            else
            {
                SetImm(token, val);
            }
        }
        else
        {
            SetReg(token, val, i);
        }
    }
}

void OpParser::SetReg(const std::string& regval, opvalue& op, unsigned int index)
{
    assert(regval.size() > 0);
    assert(index > 0 && index <= 3);

    CompileException exc("The value '" + regval + "' is not a valid register. Registers are values of 'r0'-'r15'");

    if (regval[0] != REG_CHAR)
        throw exc;

    std::string regnum = regval.substr(1);

    uint32_t val;
    try
    {
        val = utils::ParseUnsignedInt(regnum);
        //val = std::stoi(regnum, &val, 10);
    }
    catch (utils::StringParseException)
    {
        throw exc;
    }
    catch (std::invalid_argument)
    {
        throw exc;
    }
    catch (std::out_of_range)
    {
        throw exc;
    }

    if (val < 0 || val > 15)
        throw exc;

    uint8_t val8 = static_cast<uint8_t>(val);
    switch (index)
    {
        case 1:
            op.slot1 = val8;
            break;
        case 2:
            op.slot2 = val8;
            break;
        case 3:
            op.slot3.reg = val8;
            break;
        default:
            DEATH();
            break;
    }

}

void OpParser::SetImm(const std::string& immval, opvalue& op)
{
    assert(immval.size() > 0);

    CompileException exc("The value '" + immval + "' is not a valid immediate value.");

    uint16_t val;
    try
    {
        val = GetShortValue(immval);//utils::ParseUnsignedInt(immval);
        // base 0 = auto detect dec, hex, octal
        //val = std::stoi(immval, &val, 0);
    }
    catch (utils::StringParseException)
    {
        throw exc;
    }
    catch (std::invalid_argument)
    {
        throw exc;
    }
    catch (std::out_of_range)
    {
        throw exc;
    }

    if (val < 0 || val >= 1 << 16)
        throw exc;

    op.slot3.imm = static_cast<uint16_t>(val);
}

static inline std::string GetLabelName(const std::string& label)
{
    assert(IsLabel(label));
    assert(label.size() != 0);
    // Actually handle label
    std::string labelName = label.substr(1);

    if (labelName.size() == 0)
        throw CompileException("Error: Invalid syntax ':'.");

    return labelName;
}

void OpParser::AddLabel(const std::string& labelToken)
{
    std::string label = GetLabelName(labelToken);

    auto iterCheck = labels.find(label);
    if (iterCheck != labels.end())
        throw CompileException("The label '" + label + "' has already been defined.");

    int lineNum = ops.size();
    labels[label] = lineNum;

    auto iter = labelsNeeded.find(label);
    if (iter == labelsNeeded.end())
        return;

    uint16_t ulineNum = static_cast<uint16_t>(lineNum);
    for (int line : iter->second)
    {
        opvalue& op = ops[line];
        op.slot3.imm = ulineNum;
    }

    labelsNeeded.erase(iter);
}

void OpParser::SetLabelImm(const std::string& labelToken, opvalue& val)
{
    std::string label = GetLabelName(labelToken);

    auto iter = labels.find(label);
    if (iter == labels.end())
    {
        // Label doesn't exist yet, so put it in the store of what labels to replace
        int lineNum = ops.size();
        labelsNeeded[label].push_back(lineNum);
        return;
    }

    uint16_t labelAddr = static_cast<uint16_t>(iter->second);
    val.slot3.imm = labelAddr;
}

