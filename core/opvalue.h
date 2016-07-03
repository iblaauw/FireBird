#pragma once

#include "common.h"

union op_slot3
{
    uint16_t imm;
    uint8_t reg:4;
};

// Note that these are listed in backwards order because
//  of little endianess
// This way slot3 is at offset 0 and op is at the large offset
struct opvalue
{
    op_slot3 slot3;
    uint8_t slot2:4;
    uint8_t slot1:4;
    uint8_t argFlag:1;
    uint8_t opFlag:2;
    uint8_t op:5;
};

static_assert(sizeof(opvalue) == 4, "opvalue is not a 32 bit struct!");
