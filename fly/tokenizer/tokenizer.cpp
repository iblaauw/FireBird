#include "tokenizer.h"

#include <string>

using Token = firefly::token::Token;
using Tokenizer = firefly::token::Tokenizer;
using TokenType = firefly::token::TokenType;

/*static*/ std::map<char,TokenType> Tokenizer::singleTypeMap =
{
    { ';', firefly::token::SEMICOLON     },
    { '(', firefly::token::PAREN_OPEN    },
    { ')', firefly::token::PAREN_CLOSE   },
    { '{', firefly::token::BRACKET_OPEN  },
    { '}', firefly::token::BRACKET_CLOSE },
    { '+', firefly::token::PLUS          },
    { '-', firefly::token::MINUS         },
    { '/', firefly::token::SLASH         },
    { '*', firefly::token::STAR          },
    { '=', firefly::token::EQUALS        },
    { '"', firefly::token::QUOTE         },
    { ',', firefly::token::COMMA         },
};

/*static*/ std::map<StringView,TokenType> Tokenizer::multiTypeMap =
{
    { "return"_sv , firefly::token::RETURN },
    { "if"_sv     , firefly::token::IF     },
    { "else"_sv   , firefly::token::ELSE   },
};

Tokenizer::Tokenizer(std::istream& input) : input(input), tokens(),
    curPos(0), lineNum(0) {}


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
    return true;
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

char Tokenizer::GrabChar(bool* outEndFile)
{
    *outEndFile = false;

    if (curPos == currentLine.Size())
    {
        curPos++;
        return '\n';
    }

    // The current line has already been parsed
    if (curPos > currentLine.Size())
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

    char val = currentLine[curPos];
    curPos++;
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
    auto iter = singleTypeMap.find(c);
    if (iter != singleTypeMap.end())
    {
        EndUnknown();
        AddToken(iter->second);
        return true;
    }

    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
        if (hasUnknown)
        {
            EndUnknown();
            return true;
        }
        return false;
    default:
        AddToUnknown();
        return false;
    }
}

void Tokenizer::AddToUnknown()
{
    if (hasUnknown)
        return;

    hasUnknown = true;
    unkStart = curPos - 1; // curPos was already incremented
}

void Tokenizer::EndUnknown()
{
    if (!hasUnknown)
        return;

    int unkEnd = curPos - 1; // curPos was already incremented
    StringView value = currentLine.Slice(unkStart, unkEnd);

    // Is this 'unknown' a keyword?
    auto iter = multiTypeMap.find(value);
    if (iter != multiTypeMap.end())
    {
        tokens.emplace_back(iter->second);
    }
    else
    {
        tokens.emplace_back(value);
    }

    hasUnknown = false;
    unkStart = -1;
}

void Tokenizer::AddToken(TokenType type)
{
    tokens.emplace_back(type);
}

