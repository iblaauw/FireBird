#pragma once

#include <StringView.h>

namespace firefly
{
namespace token
{
    enum TokenType
    {
        UNKNOWN = -1,

        // Single Char
        SEMICOLON,
        PAREN_OPEN,
        PAREN_CLOSE,
        BRACKET_OPEN,
        BRACKET_CLOSE,
        PLUS,
        MINUS,
        SLASH,
        STAR,
        EQUALS,
        QUOTE,
        COMMA,

        // Multi Char
        RETURN,
        IF,
        ELSE,
    };

    class Token
    {
    public:

        TokenType type;
        StringView val;
        int linenum;

        Token(TokenType type) : type(type) {}
        Token(StringView val) : type(UNKNOWN), val(val) {}
        const std::string& ToString() const;
    };
}
}
