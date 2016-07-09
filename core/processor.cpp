#include "processor.h"
#include "opcodes.h"
#include "message_exception.h"
#include <cstring>
#include <iostream>

#define PC_INC_AMOUNT 4


Processor::Processor(Memory* memory) : _memory(memory)
{
    memset(regs, 0, sizeof(regs));
    pc = 0;
}

void Processor::Start(word address)
{
    pc = address;
    Execute();
}

#define DO_SIMPLE_TRIPLE(opcode, operation) case opcode: *dest = arg1 operation arg2; break

inline void DoTripleOp(OpWrapper& wrapper, word* regs)
{
    word* dest = regs + wrapper.Slot1();
    word arg1 = regs[wrapper.Slot2()];
    word arg2 = wrapper.ArgFlag() ? wrapper.Immediate() : regs[wrapper.Reg3()];
    switch (wrapper.Opcode())
    {
        // DO_SIMPLE_TRIPLE(OP_ADD, +);

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
        case OP_AND:
            *dest = arg1 & arg2;
            break;
        case OP_OR:
            *dest = arg1 | arg2;
            break;
        case OP_XOR:
            *dest = arg1 ^ arg2;
            break;
        case OP_LSHIFT:
            *dest = arg1 << arg2;
            break;
        case OP_RSHIFT:
            *dest = arg1 >> arg2;
            break;
        default: // Should never reach
            throw std::string("Unreachable");
    }
}

inline void DoDoubleOp(OpWrapper& op, word* regs)
{
    word* dest = regs + op.Slot1();
    word arg = op.ArgFlag() ? op.Immediate() : regs[op.Slot2()];
    switch (op.Opcode())
    {
        case OP_MOVE:
            *dest = arg;
            break;
        default: // Should never reach
            throw std::string("Unreachable");
    }
}

inline void DoSyscall(OpWrapper& op, word* regs)
{
    // For right now, just print something:
    word val = regs[op.Slot1()];
    std::cout << "syscall print: " << val << std::endl;
}

inline void DoMemory(OpWrapper& op, word* regs, Memory* mem)
{
    word* dest = regs + op.Slot1();
    word arg = op.ArgFlag() ? op.Immediate() : regs[op.Slot2()];
    byte* raw = mem->GetAddress(arg);
    word* memSpot = reinterpret_cast<word*>(raw);
    switch (op.Opcode())
    {
        case OP_LOAD:
            *dest = *memSpot;
        case OP_STORE:
            *memSpot = *dest;
        default:
            DEATH();
    }
}

inline word GetBranchLoc(OpWrapper& op, word* regs)
{
    return op.ArgFlag() ? op.Immediate() : regs[op.Slot1()];
}

void Processor::Execute()
{
    while(true)
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
            case OP_AND:
            case OP_OR:
            case OP_XOR:
            case OP_LSHIFT:
            case OP_RSHIFT:
                DoTripleOp(wrapper, regs);
                break;
            case OP_MOVE:
                DoDoubleOp(wrapper, regs);
                break;
            case OP_LOAD:
            case OP_STORE:
                DoMemory(wrapper, regs, _memory);
                break;
            case OP_BRANCH:
                pc = 4*GetBranchLoc(wrapper, regs) - PC_INC_AMOUNT;
                break;
            case OP_SYSCALL:
                DoSyscall(wrapper, regs);
                break;
            case OP_RETURN:
                std::cout << "Program Finished" << std::endl;
                return;
            default:
                std::cout << "Warning: Did not recognize opcode "
                          << std::hex << opcode
                          << " at address " << pc
                          << ". Ignoring it." << std::dec
                          << std::endl;
                break;
        }

        pc += PC_INC_AMOUNT;
    }
}


