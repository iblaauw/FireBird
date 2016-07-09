#include "opparser.h"
#include "common.h"
#include "opcodes.h"
#include "split.h"
#include "stringutils.h"
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

#define COMMENT_CHAR '#'
#define LABEL_CHAR ':'
#define REG_CHAR 'r'

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
    { "MOVE"   , OP_MOVE    },
    { "SYSCALL", OP_SYSCALL },
    { "RETURN" , OP_RETURN  },
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
    { OP_MOVE   , 2 },
    { OP_SYSCALL, 1 },
    { OP_RETURN,  1 },
};

void OpParser::Parse(const std::string& line)
{
    std::string pure_line = RemoveComments(line);

    Tokens tokens = patch::split(pure_line, ' ');
    if (tokens.size() == 0) // Empty line
        return;

    if (tokens[0][0] == LABEL_CHAR)
    {
        if (tokens.size() != 1)
            throw CompileException(
                "Error: a label must be on its own line.\n");

        // Actually handle label
    }

    ParseOp(tokens);
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

    std::string opcode = tokens[0];
    auto iter = nameToOp.find(opcode);
    if (iter == nameToOp.end())
        throw CompileException("Error: unknown opcode '" + opcode + "'");

    opvalue op;
    memset(&op, 0, sizeof(opvalue));
    op.op = iter->second;

    std::cout << "iter->first " << iter->first << std::endl;
    std::cout << "iter->second " << (int)iter->second << std::endl;


    unsigned int numArgs = opTypeMap.at(iter->second);
    HandleOpArgs(op, tokens, numArgs);

    ops.push_back(op);
}

static bool IsRegister(const std::string& token)
{
    if (token.size() == 0)
        return false;

    return token[0] == REG_CHAR;
}

void OpParser::HandleOpArgs(opvalue& val, const Tokens& tokens, unsigned int numArgs)
{
    assert(numArgs >= 0 && numArgs <= 3);

    std::cout << "Num args: " << numArgs << std::endl;
    std::cout << "Tokens Size: " << tokens.size() << std::endl;

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
            SetImm(token, val);
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


