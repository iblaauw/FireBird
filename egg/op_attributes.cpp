#include "op_attributes.h"
#include "opcodes.h"

#define ATTR(op,flag,numargs) OpAttributes::AttributeSet { \
    (op), (flag), (numargs) \
}

#define SLOT(name, op, flag, numargs) { (#name), ATTR((OP_ ## op), (flag), (numargs)) }

/* static */ const std::map<std::string, OpAttributes::AttributeSet> OpAttributes::masterSet =
{
// Standard

   /*      Name     OP     Flag  NumArgs */

     SLOT( NOOP,    NOOP,    0,    0 ),
     SLOT( ADD,     ADD,     0,    3 ),
     SLOT( SUB,     SUB,     0,    3 ),
     SLOT( MULT,    MULT,    0,    3 ),
     SLOT( DIV,     DIV,     0,    3 ),
     SLOT( MOD,     MOD,     0,    3 ),
     SLOT( AND,     AND,     0,    3 ),
     SLOT( OR,      OR,      0,    3 ),
     SLOT( XOR,     XOR,     0,    3 ),
     SLOT( LSHIFT,  LSHIFT,  0,    3 ),
     SLOT( RSHIFT,  RSHIFT,  0,    3 ),
     SLOT( MOVE,    MOVE,    0,    2 ),
     SLOT( LOAD,    LOAD,    0,    2 ),
     SLOT( STORE,   STORE,   0,    2 ),
     SLOT( EQUAL,   EQUAL,   0,    2 ),
     SLOT( GREATER, GREATER, 0,    2 ),
     SLOT( LESS,    LESS,    0,    2 ),
     SLOT( ISZERO,  ISZERO,  0,    1 ),
     SLOT( BRANCH,  BRANCH,  0,    1 ),
     SLOT( PUSH,    PUSH,    0,    1 ),
     SLOT( POP,     POP,     0,    1 ),
     SLOT( POPX,    POPX,    0,    1 ),
     SLOT( ARG,     ARG,     0,    2 ),
     SLOT( CALL,    CALL,    0,    1 ),
     SLOT( RETURN,  RETURN,  0,    0 ),
     SLOT( SYSCALL, SYSCALL, 0,    1 ),

// Custom

   /*      Name        OP      Flag   NumArgs */

    SLOT( ARG_FUNC,    ARG,      1,      2 ),
    SLOT( FUNC_ARG,    ARG,      1,      2 ),
    SLOT( NOT_EQUAL,   EQUAL,    1,      2 ),
    SLOT( EQUAL_NOT,   EQUAL,    1,      2 ),
    SLOT( GREATER_EQ,  LESS,     1,      2 ),
    SLOT( LESS_EQ,     GREATER,  1,      2 ),
    SLOT( ISZERO_NOT,  ISZERO,   1,      1 ),
    SLOT( ISNOTZERO,   ISZERO,   1,      2 ),
    SLOT( BRANCH_IF,   BRANCH,   1,      1 ),

// FLoat ops
    SLOT( ADD_F,       ADD,      1,      3 ),
    SLOT( SUB_F,       SUB,      1,      3 ),
    SLOT( MULT_F,      MULT,     1,      3 ),
    SLOT( DIV_F,       DIV,      1,      3 ),
    SLOT( SYSCALL_F,   SYSCALL,  1,      1 ),
};

/* static */ bool OpAttributes::TryGetAttributes(
    const std::string& token,
    OpCode* outOp,
    OpFlag* outFlag,
    uint8_t* outNumArgs)
{
    auto iter = masterSet.find(token);

    if (iter == masterSet.end())
        return false;

    const AttributeSet& attr = iter->second;
    *outOp = attr.op;
    *outFlag = attr.flag;
    *outNumArgs = attr.numArgs;

    return true;
}

