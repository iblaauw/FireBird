#include "memory.h"
#include <cstdlib>
#include <string>

#define MEM_SIZE (1024*1024)

Memory::Memory()
{
    memory = new byte[MEM_SIZE];
}

Memory::Memory(Memory&& other)
{
    memory = other.memory;
    other.memory = nullptr;
}

Memory::~Memory()
{
    delete[] memory;
    memory = nullptr;
}

byte* Memory::GetRawMemory()
{
    return memory;
}

byte* Memory::GetAddress(uint32_t address)
{
    if (address % 4 != 0)
    {
        std::cout << "Error: address is not aligned: " << address << std::endl;
        exit(-1);
    }
    return memory + address;
}

void Memory::LoadFile(std::ifstream& file)
{
    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    if (length >= MEM_SIZE)
    {
        std::cout << "Error: file is too large" << std::endl;
        exit(-1);
        return;
    }

    char* data = reinterpret_cast<char*>(memory);
    file.read(data, length);

    if (!file)
    {
        std::cout << "Error: could not load data for the file" << std::endl;
        std::cout.flush();
        exit(-1);
    }
}


