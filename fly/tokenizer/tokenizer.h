#pragma once

#include "token.h"
#include <StringView.h>

#include <iostream>
#include <deque>

namespace firefly
{
namespace token
{
    class Tokenizer
    {
    private:
        std::istream& input;
        std::deque<Token> tokens; // TODO: replace with a linked list?

        int curPos;
        int lineNum; // For output purposes
        StringView currentLine;

        bool hasUnknown = false;;
        int unkStart;
    public:
        Tokenizer(std::istream& input) : input(input), tokens() {}
        bool Advance();
        void Consume();

        const Token& Current();
        const Token& At(int index);

        inline int size() const { return tokens.size(); }

    private:
        char GrabChar(bool* outEndFile);
        bool GrabLine();
        bool HandleChar(char c);

        void AddToUnknown(char val);
        void EndUnknown();
    };
}
}
