#include "common.h"
#include "opvalue.h"
#include "memory.h"
#include "OpWrapper.h"
#include "processor.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        cout << "Error: please specify a file" << endl;
        return -1;
    }

    char* filename = argv[1];
    ifstream inFile;
    inFile.open(filename, ios::in | ios::binary);

    if (!inFile)
    {
        cout << "Error: could not open file " << filename << endl;
        cout.flush();
        return -1;
    }

    // Set up FireBird memory
    Memory memory;

    // Load file into memory
    memory.LoadFile(inFile);
    inFile.close();

    Processor processor(&memory);
    processor.Start(0);
    return 0;


    OpWrapper wrapper(memory.GetAddress(0));

    cout << "OpCode: " << (unsigned int)wrapper.Opcode() << endl;
    cout << "String: " << wrapper.GetString() << endl;
    cout << "Hex: " << std::hex << wrapper.GetInt() << std::dec << endl;


    byte data[4];
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    data[0] = 5;
    data[1] = 0xF0;
    byte* raw = data;
    opvalue* op2 = reinterpret_cast<opvalue*>(raw);
    cout << "op2 op test: " << (int)(op2->op) << endl;
    cout << "\t" << std::hex << (int)(op2->slot3.imm) << std::dec << endl;
    cout << "\t" << (int)(op2->slot3.reg) << endl;

    cout << "Beginning asserts..." << endl;

    assert(wrapper.Opcode() == 12);
    assert(wrapper.OpFlag() == 2);
    assert(wrapper.ArgFlag() == false);
    assert(wrapper.Slot1() == 6);
    assert(wrapper.Slot2() == 3);
    assert(wrapper.Immediate() == 0x6261);
    assert(wrapper.Reg3() == 1);

    cout << "Asserts successful!" << endl;

    return 0;
}
