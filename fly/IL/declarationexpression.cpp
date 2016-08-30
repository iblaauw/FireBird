#include "declarationexpression.h"

using namespace firefly::IL;

void DeclarationExpression::DebugPrint(TreePrinter& printer) const
{
    printer.Print(type);
    printer.Print(" ");
    printer.Print(name);
}

