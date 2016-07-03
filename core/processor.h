#pragma once

#include "common.h"
#include "memory.h"
#include "OpWrapper.h"

class Processor
{
private:
    Memory* _memory;

    word regs[16];
    word pc;

public:
    Processor(Memory* memory);

    // Does not return
    void Start(word address);

private:
    OpWrapper LoadInstruction();
    void Execute();
    void DoTripleOp(OpWrapper& wrapper);
    void DoDoubleOp(OpWrapper& op);
    void DoSyscall(OpWrapper& op);
};
