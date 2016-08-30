#pragma once

#include <StringView.h>

#include <iostream>

namespace firefly
{
namespace IL
{
    class TreePrinter
    {
    private:
        int indentLevel = 0;
        bool newlined = true;
    public:
        template <class T>
        void Print(const T& thing)
        {
            PrintIndent();
            std::cout << thing;
        }

        void PrintLine();
        void Increase();
        void Decrease();

    private:
        void PrintIndent();
    };
}
}
