#pragma once

#include "common.h"
#include "opvalue.h"
#include <string>

class OpWrapper
{
public:
    OpWrapper(const byte* op);
    inline uint8_t Opcode() { return opval->op; }
    inline uint8_t OpFlag() { return opval->opFlag; }
    inline bool ArgFlag() { return opval->argFlag != 0; }
    inline uint8_t Slot1() { return opval->slot1; }
    inline uint8_t Slot2() { return opval->slot2; }
    inline uint16_t Immediate() { return opval->slot3.imm; }
    inline uint8_t Reg3() { return opval->slot3.reg; }

    std::string GetString() { return std::string(reinterpret_cast<const char*>(opval), 4); }
    uint32_t GetInt() { return *(reinterpret_cast<const uint32_t*>(opval)); }

private:
    const opvalue* opval;
};
