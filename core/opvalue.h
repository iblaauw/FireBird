#pragma once

#include <cstdint>

union op_slot3
{
    uint16_t imm;
    uint8_t reg;
};

struct opvalue
{
    uint8_t op:5;
    uint8_t opFlag:2;
    uint8_t argFlag:1;
    uint8_t slot1:4;
    uint8_t slot2:4;
    op_slot3 slot3;
};
