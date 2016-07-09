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

#define OP_BRANCH   0x14

#define OP_PUSH     0x15
#define OP_POP      0x16

#define OP_RETURN   0x1B
#define OP_SYSCALL  0x1F

