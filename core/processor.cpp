#include "processor.h"
#include <cstring>
#include <iostream>

#define OP_NOOP     0x0
#define OP_ADD      0xA
#define OP_SUB      0xB
#define OP_MULT     0xC
#define OP_DIV      0xD
#define OP_MOD      0xE
#define OP_AND      0x1
#define OP_OR       0x2
#define OP_XOR      0x3
#define OP_MOVE     0x3
#define OP_SYSCALL  0x1F

Processor::Processor(Memory* memory) : _memory(memory)
{
    memset(regs, 0, sizeof(regs));
    pc = 0;
}

void Processor::Start(word address)
{
    pc = address;
    while (true)
    {
        Execute();
    }
}

OpWrapper Processor::LoadInstruction()
{
    byte* data = _memory->GetAddress(pc);
    return OpWrapper(data);
}

void Processor::Execute()
{
    OpWrapper wrapper = LoadInstruction();
    word opcode = wrapper.Opcode();
    switch (opcode)
    {
        case OP_NOOP:
            break;
        case OP_ADD:
        case OP_SUB:
        case OP_MULT:
        case OP_DIV:
        case OP_MOD:
            DoTripleOp(wrapper);
            break;
        case OP_MOVE:
            DoDoubleOp(wrapper);
        case OP_SYSCALL:
            DoSyscall(wrapper);
            break;
        default:
            std::cout << "Warning: Did not recognize opcode "
                      << std::hex << opcode
                      << " at address " << pc
                      << ". Ignoring it." << std::dec
                      << std::endl;
            break;
    }

    pc += 4;
}

void Processor::DoTripleOp(OpWrapper& wrapper)
{
    word* dest = regs + wrapper.Slot1();
    word arg1 = wrapper.Slot2();
    word arg2 = wrapper.ArgFlag() ? wrapper.Immediate() : wrapper.Reg3();
    switch (wrapper.Opcode())
    {
        case OP_ADD:
            *dest = arg1 + arg2;
            break;
        case OP_SUB:
            *dest = arg1 - arg2;
            break;
        case OP_MULT:
            *dest = arg1 * arg2;
            break;
        case OP_DIV:
            *dest = arg1 / arg2;
            break;
        case OP_MOD:
            *dest = arg1 % arg2;
            break;
        default: // Should never reach
            throw std::string("Unreachable");
    }
}

void Processor::DoDoubleOp(OpWrapper& op)
{
    word* dest = regs + op.Slot1();
    word arg = op.ArgFlag() ? op.Immediate() : op.Slot2();
    switch (op.Opcode())
    {
        case OP_MOVE:
            *dest = arg;
            break;
        default: // Should never reach
            throw std::string("Unreachable");
    }
}

void Processor::DoSyscall(OpWrapper& op)
{
    // For right now, just print something:
    word val = regs[0];
    std::cout << "syscall print: " << val << std::endl;
}

