#include "opparser.h"
#include "common.h"
#include "opcodes.h"
#include "split.h"

/*static*/ std::map<std::string, uint8_t> OpParser::nameToOp =
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
};

void OpParser::Parse(const std::string& line)
{
    std::vector<std::string> tokens = patch::split(line, ' ');
    if (tokens.size() == 0)
        throw CompileException("Error");

}
