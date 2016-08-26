#pragma once

#include "token.h"

#include <string>
#include <exception>

namespace firefly
{
namespace frontend
{
    class FrontEndException : public std::exception
    {
    private:
        std::string message;
    public:
        FrontEndException(const std::string& message);
        FrontEndException(const std::string& message, const token::Token& token);

        const char* what() const throw() override { return message.c_str(); }
    };
}
}

