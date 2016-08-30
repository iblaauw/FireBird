#include "functionexpression.h"

using namespace firefly::IL;

void FunctionExpression::DebugPrint(TreePrinter& printer) const
{
    printer.Print(returnType);
    printer.Print(" ");
    printer.Print(name);
    printer.Print("(");
    printer.PrintLine();

    printer.Increase();

    for (auto& decl : args)
    {
        decl->DebugPrint(printer);
        printer.Print(",");
    }
    printer.Print(") {");
    printer.PrintLine();

    for (auto& expr : expressions)
    {
        expr->DebugPrint(printer);
        printer.PrintLine();
    }

    printer.Decrease();
    printer.Print("}");
}



