#include "assignmentexpression.h"

using namespace firefly::IL;

void AssignmentExpression::DebugPrint(TreePrinter& printer) const
{
    variable->DebugPrint(printer);
    printer.Print(" = ");
    value->DebugPrint(printer);
}
