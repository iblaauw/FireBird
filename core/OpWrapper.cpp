#include "OpWrapper.h"

OpWrapper::OpWrapper(const byte* op)
{
    opval = reinterpret_cast<const opvalue*>(op);
}

