#include "treeprinter.h"

using namespace firefly::IL;

void TreePrinter::PrintLine()
{
    std::cout << std::endl;
    newlined = true;
}

void TreePrinter::Increase() { indentLevel++; }
void TreePrinter::Decrease() { indentLevel--; }

void TreePrinter::PrintIndent()
{
    if (!newlined)
        return;

    for (int i = 0; i < indentLevel; i++)
    {
        std::cout << "\t";
    }

    newlined = false;
}

