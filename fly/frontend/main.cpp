#include "tokenizer.h"
#include "builder.h"

#include <StringView.h>
#include <iostream>
#include <fstream>

using namespace firefly;

#define PRINT_LINE(value) std::cout << #value << std::endl

void PrintToken(const token::Token& tok)
{
    switch (tok.type)
    {
    case token::UNKNOWN:
        std::cout << "UNK: " << tok.val << std::endl;
        break;
    case token::SEMICOLON:
        PRINT_LINE(SEMICOLON);
        break;
    case token::PAREN_OPEN:
        PRINT_LINE(PAREN_OPEN);
        break;
    case token::PAREN_CLOSE:
        PRINT_LINE(PAREN_CLOSE);
        break;
    case token::BRACKET_OPEN:
        PRINT_LINE(BRACKET_OPEN);
        break;
    case token::BRACKET_CLOSE:
        PRINT_LINE(BRACKET_CLOSE);
        break;
    case token::PLUS:
        PRINT_LINE(PLUS);
        break;
    case token::MINUS:
        PRINT_LINE(MINUS);
        break;
    case token::SLASH:
        PRINT_LINE(SLASH);
        break;
    case token::STAR:
        PRINT_LINE(STAR);
        break;
    case token::EQUALS:
        PRINT_LINE(EQUALS);
        break;
    case token::QUOTE:
        PRINT_LINE(QUOTE);
        break;
    case token::RETURN:
        PRINT_LINE(RETURN);
        break;
    case token::IF:
        PRINT_LINE(IF);
        break;
    case token::ELSE:
        PRINT_LINE(ELSE);
        break;
    default:
        std::cout << "Error: unkown token type " << tok.type << std::endl;
        return;
    }
}

void flushTokenizer(token::Tokenizer& tokenizer)
{
    while (tokenizer.size() > 0)
    {
        const token::Token& tok = tokenizer.Current();
        PrintToken(tok);
        tokenizer.Consume();
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Wrong number of arguments!" << std::endl;
        return -1;
    }

    char* filename = argv[1];
    std::ifstream infile;
    infile.open(filename);
    if (!infile)
    {
        std::cout << "Error: Couldn't open file " << filename << std::endl;
        return -1;
    }

    token::Tokenizer tokenizer(infile);

//    while (true)
//    {
//        bool success = tokenizer.Advance();
//
//        flushTokenizer(tokenizer);
//
//        if (!success) // EOF
//            break;
//    }

    frontend::Builder builder(tokenizer);
    builder.Build();
    builder.DebugPrint();

    return 0;
}

