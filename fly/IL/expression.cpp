#include "expression.h"
#include "ilexception.h"

void firefly::IL::AssertNonNull(ExpressionPtr ptr)
{
    if (ptr == nullptr)
        throw ILException("Null Expression");
}

