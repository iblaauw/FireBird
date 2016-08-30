#include "operandexpression.h"

#include "hiddenerror.h"

using namespace firefly::IL;

/*static*/ OperandExpressionPtr OperandExpression::Error(const std::string& message)
{
    return CreateError<OperandExpression>(message);
}

