#include "function.h"
#include "ilexception.h"

firefly::IL::Function::AddExpression(ExpressionPtr exp)
{
    AssertNonNull(exp);
    expressions.push_back(exp);
}

firefly::IL::Function::iter_type firefly::IL::Function::begin()
{
    return expressions.begin();
}

firefly::IL::Function::iter_type firefly::IL::Function::end()
{
    return expressions.end();
}
