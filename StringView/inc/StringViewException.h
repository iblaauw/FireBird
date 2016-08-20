#pragma once

#include <exception>

class StringViewException : public std::exception
{
private:
    std::string message;
public:
    StringViewException(const char* message) : message(message) {}
    StringViewException(std::string message) : message(message) {}
    const char* what() const throw() override { return message.c_str(); }
};

