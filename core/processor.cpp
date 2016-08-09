#include "processor.h"
#include "opcodes.h"
#include "message_exception.h"
#include "intvals.h"
#include "testframework.h"
#include <cstring>
#include <iostream>
#include <cassert>

#define PC_INC_AMOUNT 4

#define STACK_START 1000


Processor::Processor(Memory* memory) :
    _memory(memory),
    pc(0),
    stackPtr(STACK_START),
    framePtr(STACK_START),
    link(0),
    compFlag(false)
{
    memset(regs, 0, sizeof(regs));
}

void Processor::Start(word address)
{
    Execute();
}

template <class Type>
inline bool ArithmeticOp(uint8_t opcode, Type* dest, Type arg1, Type arg2)
{
    switch (opcode)
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
        default:
            return false;
    }
    return true;
}

template <class Type>
inline bool BinaryOp(uint8_t opcode, Type* dest, Type arg1, Type arg2)
{
    switch (opcode)
    {
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
        default:
            return false;
    }

    return true;
}

#define DO_SIMPLE_TRIPLE(opcode, operation) case opcode: *dest = arg1 operation arg2; break

inline void DoTripleOp(OpWrapper& wrapper, word* regs)
{
    word* dest = regs + wrapper.Slot1();
    word arg1 = regs[wrapper.Slot2()];
    word arg2 = wrapper.ArgFlag() ? wrapper.Immediate() : regs[wrapper.Reg3()];

    bool success = ArithmeticOp(wrapper.Opcode(), dest, arg1, arg2);
    if (success)
        return;

    // else it was a binary op
    success = BinaryOp(wrapper.Opcode(), dest, arg1, arg2);

    if (!success)
    {
        std::cout << "ERROR: unknown arithmetic opcode '" << wrapper.Opcode() << "'. Ignoring it." << std::endl;
    }
}

inline void DoTripleFloatOp(OpWrapper& op, word* regs)
{
    static_assert(sizeof(float) == sizeof(word), "Invalid size mismatch!");
    assert(op.OpFlag() == 1);

    word* dest_w = regs + op.Slot1();
    word arg1_w = regs[op.Slot2()];
    word arg2_w = op.ArgFlag() ? op.Immediate() : regs[op.Reg3()];

    float* dest = reinterpret_cast<float*>(dest_w);
    float arg1 = *(reinterpret_cast<float*>( &arg1_w ));
    float arg2 = *(reinterpret_cast<float*>( &arg2_w ));

    // Floating point only does arithmetic ops, not binary ones
    bool success = ArithmeticOp(op.Opcode(), dest, arg1, arg2);
    if (!success)
    {
        std::cout << "ERROR: unknown floating-point arithmetic opcode '" << op.Opcode() << "'. Ignoring it." << std::endl;
    }
}

inline void DoDoubleOp(OpWrapper& op, word* regs)
{
    word* dest = regs + op.Slot1();
    word arg = op.ArgFlag() ? op.Immediate() : regs[op.Slot2()];
    switch (op.Opcode())
    {
        case OP_MOVE:
            if (!op.OpFlag())
            {
                *dest = arg;
            }
            else
            {
                *dest &= 0x0000FFFF;
                *dest |= arg << 16;
            }
            break;
        default: // Should never reach
            throw std::string("Unreachable");
    }
}

inline void DoSyscall(OpWrapper& op, word* regs)
{
    // For right now, just print something:
    word val = op.ArgFlag() ? op.Immediate() : regs[op.Slot1()];
    std::string toPrint;
    if (op.OpFlag() == 0)
    {
        toPrint = patch::to_string(val);
    }
    else
    {
        float x = *(reinterpret_cast<float*>(&val));
        toPrint = patch::to_string(x);
    }

    std::cout << "syscall print: " << toPrint << std::endl;

    auto testing = TestFramework::GetInstance();
    if (testing)
        testing->LogOutput(toPrint);
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
            break;
        case OP_STORE:
            *memSpot = *dest;
            break;
        default:
            DEATH();
    }
}

inline void DoComp(OpWrapper& op, word* regs, bool* compFlag)
{
    word* dest = regs + op.Slot1();
    word arg = op.ArgFlag() ? op.Immediate() : regs[op.Slot2()];

    switch (op.Opcode())
    {
        case OP_EQUAL:
            *compFlag = *dest == arg;
            break;
        case OP_GREATER:
            *compFlag = *dest > arg;
            break;
        case OP_LESS:
            *compFlag = *dest < arg;
            break;
        default:
            DEATH();
    }

    if (op.OpFlag() == 1)
        *compFlag = !(*compFlag);
}

inline void DoZeroComp(OpWrapper& op, word* regs, bool* compFlag)
{
    if (op.Opcode() != OP_ISZERO)
        DEATH();

    word arg = op.ArgFlag() ? op.Immediate() : regs[op.Slot1()];
    *compFlag = arg == 0;
}

void Processor::Push(word val)
{
    byte* buffer = _memory->GetAddress(stackPtr);
    word* slot = reinterpret_cast<word*>(buffer);
    *slot = val;
    stackPtr += sizeof(word);
}

void Processor::Pop(word* popTo)
{
    stackPtr -= sizeof(word);
    byte* buffer = _memory->GetAddress(stackPtr);
    word* slot = reinterpret_cast<word*>(buffer);
    *popTo = *slot;
}

void Processor::PushForCall()
{
    Push(framePtr);
    Push(link);
}

void Processor::PopForReturn()
{
    Pop(&link);
    Pop(&framePtr);
}

void Processor::DoStackOp(OpWrapper& op)
{
    word* regSlot = regs + op.Slot1();
    word val = op.ArgFlag() ? op.Immediate() : *regSlot;

    switch (op.Opcode())
    {
        case OP_PUSH:
            Push(val);
            break;
        case OP_POP:
            Pop(regSlot);
            break;
        case OP_POPX:
            stackPtr -= val;
            break;
        case OP_CALL:
            PushForCall(); // Save everything
            framePtr = stackPtr;
            link = pc;
            pc = 4*val - PC_INC_AMOUNT;
            break;
        case OP_RETURN:
            pc = link;
            stackPtr = framePtr;
            PopForReturn(); // Restore everything
            break;
        default:
            DEATH();
    }
}

void Processor::DoArgOp(OpWrapper& op)
{
    assert(op.Opcode() == OP_ARG);

    word* dest = regs + op.Slot1();
    word val = op.ArgFlag() ? op.Immediate() : regs[op.Slot2()];

    if (op.OpFlag() == 1) // Get a function argument address
        *dest = framePtr - 12 - val*4;
    else // Get a current function local var address
        *dest = framePtr + val*4;
}

void Processor::DoBranchOp(OpWrapper& op)
{
    assert(op.Opcode() == OP_BRANCH);
    word val = op.ArgFlag() ? op.Immediate() : regs[op.Slot1()];
    if (op.OpFlag() == 1 && !compFlag)
        return;

    pc = 4*val - PC_INC_AMOUNT;
}

void Processor::Execute()
{
    while(true)
    {
        std::cout << "Address: " << pc << std::endl;
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
                if (wrapper.OpFlag() == 1)
                    DoTripleFloatOp(wrapper, regs);
                else
                    DoTripleOp(wrapper, regs);
                break;
            case OP_MOVE:
                DoDoubleOp(wrapper, regs);
                break;
            case OP_LOAD:
            case OP_STORE:
                DoMemory(wrapper, regs, _memory);
                break;
            case OP_EQUAL:
            case OP_GREATER:
            case OP_LESS:
                DoComp(wrapper, regs, &compFlag);
                break;
            case OP_ISZERO:
                DoZeroComp(wrapper, regs, &compFlag);
                break;
            case OP_BRANCH:
                DoBranchOp(wrapper);
                break;
            case OP_PUSH:
            case OP_POP:
            case OP_POPX:
            case OP_CALL:
                DoStackOp(wrapper);
                break;
            case OP_ARG:
                DoArgOp(wrapper);
                break;
            case OP_SYSCALL:
                DoSyscall(wrapper, regs);
                break;
            case OP_RETURN:
                if (framePtr == STACK_START)
                {
                    std::cout << "Program Finished" << std::endl;
                    return;
                }
                DoStackOp(wrapper);
                break;
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


