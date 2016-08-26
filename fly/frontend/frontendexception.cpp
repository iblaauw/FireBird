#include "frontendexception.h"

using Token = firefly::token::Token;
using FrontEndException = firefly::frontend::FrontEndException;

FrontEndException::FrontEndException(const std::string& message)
{
    this->message = "Error: " + message;
}


FrontEndException::FrontEndException(const std::string& message, const token::Token& token)
{
    std::string istring = std::to_string(token.linenum);
    this->message = "Error: " + message + ", occured at line " + istring;
    // TODO: print whole line
}
