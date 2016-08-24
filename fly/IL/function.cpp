#include "function.h"
#include "ilexception.h"

firefly::IL::Function::Function(
    const std::string& name, 
    FunctionSignaturePtr sig, 
    ContextPtr currentContext)
        : name(name), expressions(), signature(sig)
{
    functionContext = currentContext->CreateChild();
}

void firefly::IL::Function::AddExpression(ExpressionPtr exp)
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
