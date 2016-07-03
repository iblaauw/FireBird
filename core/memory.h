#pragma once

#include "common.h"

#include <iostream>
#include <fstream>


class Memory
{
public:
    Memory();
    Memory(Memory&& other);
    ~Memory();
    byte* GetRawMemory();
    byte* GetAddress(uint32_t address);
    void LoadFile(std::ifstream& file);
private:
    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;
    byte* memory;
};

