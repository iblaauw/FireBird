#pragma once

#define OP_NOOP     0x0
#define OP_ADD      0xA
#define OP_SUB      0xB
#define OP_MULT     0xC
#define OP_DIV      0xD
#define OP_MOD      0xE
#define OP_AND      0x1
#define OP_OR       0x2
#define OP_XOR      0x3
#define OP_LSHIFT   0x4
#define OP_RSHIFT   0x5

#define OP_MOVE     0x6

#define OP_LOAD     0x7
#define OP_STORE    0x8

//Unused            0x9
//Unused            0xF

#define OP_EQUAL    0x10
#define OP_GREATER  0x11
#define OP_LESS     0x12
#define OP_ISZERO   0x13

#define OP_BRANCH   0x14

#define OP_PUSH     0x15
#define OP_POP      0x16
#define OP_POPX     0x17
#define OP_ARG      0x18

//Unused            0x19

#define OP_CALL     0x1A
#define OP_RETURN   0x1B

//#define OP_INIT   0x1C // reserved for now

//Unused            0x1D
//Unused            0x1E

#define OP_SYSCALL  0x1F

