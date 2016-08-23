#include "tokenizer.h"

#include <string>

using Token = firefly::token::Token;
using Tokenizer = firefly::token::Tokenizer;

bool Tokenizer::Advance()
{
    bool done = false;
    while (!done)
    {
        bool eof = false;
        char c = GrabChar(&eof);

        if (eof)
        {
            if (hasUnknown)
                EndUnknown();
            return false;
        }

        done = HandleChar(c);
    }
}

void Tokenizer::Consume()
{
    tokens.pop_front();
}

const Token& Tokenizer::Current()
{
    if (tokens.empty())
        Advance();

    return tokens[0];
}

const Token& Tokenizer::At(int index)
{
    return tokens[index];
}

char GrabChar(bool* outEndFile)
{
    *outEndFile = false;

    if (curPos == currentLine.size())
    {
        curPos++;
        return '\n';
    }

    // The current line has already been parsed
    if (curPos > currentLine.size())
    {
        bool success = GrabLine();
        if (!success) // EOF
        {
            *outEndFile = true;
            return 0;
        }

        // Otherwise, a new line was grabbed, try again
        return GrabChar(outEndFile);
    }

    char val = currentLine[position];
    position++;
    return val;
}

bool Tokenizer::GrabLine()
{
    std::string line;
    bool success = (bool)std::getline(input, line);

    if (success)
    {
        currentLine = line;
        curPos = 0;
        lineNum++;
    }

    return success;
}

bool Tokenizer::HandleChar(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\n'"
        if (hasUnknown)
        {
            EndUnknown();
            return true;
        }
        return false;
    default:
        AddToUnknown(c);
        return false;
    }
}


