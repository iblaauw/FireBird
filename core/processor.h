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
    inline OpWrapper LoadInstruction()
    {
        byte* data = _memory->GetAddress(pc);
        return OpWrapper(data);
    }

    void Execute();

    //void DoTripleOp(OpWrapper& wrapper);
    //void DoDoubleOp(OpWrapper& op);
    //void DoSyscall(OpWrapper& op);
};
