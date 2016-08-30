#include "operandexpression.h"

#include "hiddenerror.h"

using namespace firefly::IL;

void OperandExpression::DebugPrint(TreePrinter& printer) const
{
    left->DebugPrint(printer);

    switch (type)
    {
        case PLUS: printer.Print('+'); break;
        case MINUS: printer.Print('-'); break;
        case TIMES: printer.Print('*'); break;
        case DIVIDES: printer.Print('/'); break;
        default:
            printer.Print("<unknown operand>");
            break;
    }

    right->DebugPrint(printer);
}


