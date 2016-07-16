#include "opparser.h"
#include "common.h"
#include "opcodes.h"
#include "split.h"
#include "stringutils.h"
#include "intvals.h"
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

#define COMMENT_CHAR '#'
#define LABEL_CHAR ':'
#define REG_CHAR 'r'

#define NAME_TO_OP_SLOT(name) { #name, OP_ ## name }

/*static*/ const std::map<std::string, uint8_t> OpParser::nameToOp =
{
    { "NOOP"   , OP_NOOP    },
    { "ADD"    , OP_ADD     },
    { "SUB"    , OP_SUB     },
    { "MULT"   , OP_MULT    },
    { "DIV"    , OP_DIV     },
    { "MOD"    , OP_MOD     },
    { "AND"    , OP_AND     },
    { "OR"     , OP_OR      },
    { "XOR"    , OP_XOR     },
    { "LSHIFT" , OP_LSHIFT  },
    { "RSHIFT" , OP_RSHIFT  },
    { "MOVE"   , OP_MOVE    },
    NAME_TO_OP_SLOT( LOAD    ),
    NAME_TO_OP_SLOT( STORE   ),
    NAME_TO_OP_SLOT( EQUAL   ),
    NAME_TO_OP_SLOT( GREATER ),
    NAME_TO_OP_SLOT( LESS    ),
    NAME_TO_OP_SLOT( ISZERO  ),
    NAME_TO_OP_SLOT( BRANCH  ),
    NAME_TO_OP_SLOT( PUSH    ),
    NAME_TO_OP_SLOT( POP     ),
    NAME_TO_OP_SLOT( POPX    ),
    NAME_TO_OP_SLOT( ARG     ),
    NAME_TO_OP_SLOT( CALL    ),
    NAME_TO_OP_SLOT( RETURN  ),
    { "SYSCALL", OP_SYSCALL },
};

/*static*/ const std::map<uint8_t, unsigned int> OpParser::opTypeMap =
{
    { OP_NOOP   , 0 },
    { OP_ADD    , 3 },
    { OP_SUB    , 3 },
    { OP_MULT   , 3 },
    { OP_DIV    , 3 },
    { OP_MOD    , 3 },
    { OP_AND    , 3 },
    { OP_OR     , 3 },
    { OP_XOR    , 3 },
    { OP_LSHIFT , 3 },
    { OP_RSHIFT , 3 },
    { OP_MOVE   , 2 },
    { OP_LOAD   , 2 },
    { OP_STORE  , 2 },
    { OP_EQUAL  , 2 },
    { OP_GREATER, 2 },
    { OP_LESS   , 2 },
    { OP_ISZERO , 1 },
    { OP_BRANCH , 1 },
    { OP_PUSH   , 1 },
    { OP_POP    , 1 },
    { OP_POPX   , 1 },
    { OP_ARG    , 2 },
    { OP_CALL   , 1 },
    { OP_RETURN , 0 },
    { OP_SYSCALL , 1 },
};

/* static */ const std::map<std::string, unsigned int> OpParser::opAdornmentMap =
{
    { "0", 0 },
    { "1", 1 },
    { "2", 2 },
    { "3", 3 },

    { "!", 1 },
    { "IF", 1 },
    { "FUNC", 1 },
};

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

    CompileException exc("Error: unknown opcode '" + opcode_name + "'");

    std::vector<std::string> namePieces = patch::split(opcode_name, '_');
    if (namePieces.size() > 2)
        throw exc;

    std::string opcode;
    if (namePieces.size() == 2)
        opcode = namePieces[0];
    else
        opcode = opcode_name;

    auto iter = nameToOp.find(opcode);
    if (iter == nameToOp.end())
        throw exc;

    opvalue op;
    memset(&op, 0, sizeof(opvalue));
    op.op = iter->second;

    // Handle adornments
    if (namePieces.size() == 2)
    {
        std::string adornment = namePieces[1];
        auto iter2 = opAdornmentMap.find(adornment);
        if (iter2 == opAdornmentMap.end())
            throw exc;

        op.opFlag = iter2->second;
    }


    std::cout << /*"iter->first " << */ iter->first << std::endl;


    unsigned int numArgs = opTypeMap.at(iter->second);
    HandleOpArgs(op, tokens, numArgs);

    ops.push_back(op);
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

    uint32_t val;
    try
    {
        val = utils::ParseUnsignedInt(immval);
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

