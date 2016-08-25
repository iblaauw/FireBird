#pragma once

#include "token.h"
#include <StringView.h>

#include <iostream>
#include <deque>
#include <map>

namespace firefly
{
namespace token
{
    class Tokenizer
    {
    private:
        static std::map<char,TokenType> singleTypeMap;
        static std::map<StringView,TokenType> multiTypeMap;

        std::istream& input;
        std::deque<Token> tokens; // TODO: replace with a linked list?

        int curPos;
        int lineNum; // For output purposes
        StringView currentLine;

        bool hasUnknown = false;;
        int unkStart;
    public:
        Tokenizer(std::istream& input);
        bool Advance();
        void Consume();

        const Token& Current();
        const Token& At(int index);

        inline int size() const { return tokens.size(); }

    private:
        char GrabChar(bool* outEndFile);
        bool GrabLine();
        bool HandleChar(char c);

        void AddToUnknown();
        void EndUnknown();

        void AddToken(TokenType type);
    };
}
}
