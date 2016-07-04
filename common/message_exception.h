#pragma once

#include <exception>

class MessageException : public std::exception
{
private:
    std::string message;
public:
    MessageException(const char* message) : message(message) {}
    MessageException(std::string message) : message(message) {}
    const char* what() const throw() override { return message.c_str(); }
};

