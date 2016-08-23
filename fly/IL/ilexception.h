#pragma once

#include <exception>
#include <string>

namespace firefly
{
namespace IL
{
    class ILException : public std::exception
    {
    public: 
        std::string message;

        ILException(std::string message) : message(message) {}
        ILException(const char* message) : message(message) {}

        const char* what() const throw() override { return message.c_str(); }
    };
}
}
