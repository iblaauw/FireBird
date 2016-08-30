#include "token.h"

#include <map>

using namespace firefly::token;

static const std::map<TokenType, std::string> stringMapping =
{
    { SEMICOLON,     ";"      },
    { PAREN_OPEN,    "("      },
    { PAREN_CLOSE,   ")"      },
    { BRACKET_OPEN,  "{"      },
    { BRACKET_CLOSE, "}"      },
    { PLUS,          "+"      },
    { MINUS,         "-"      },
    { SLASH,         "/"      },
    { STAR,          "*"      },
    { EQUALS,        "="      },
    { QUOTE,         "\""     },
    { COMMA,         ","      },
    { RETURN,        "return" },
    { IF,            "if"     },
    { ELSE,          "else"   },
};

std::string Token::ToString() const
{
    if (type == UNKNOWN)
        return static_cast<std::string>(val);

    auto iter = stringMapping.find(type);
    if (iter == stringMapping.end())
        return "<unrecognized token>";

    return iter->second;
}
