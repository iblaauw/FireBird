#pragma once

#include "IStringBuffer.h"

#include <string>
#include <memory>

class StringView
{
private:
    std::shared_ptr<IStringBuffer> _buffer;
    int _start;
    int _end;
public:
    StringView(const std::string& str);
    StringView(const char* str);

    StringView(const StringView& other); // Copy constructor
    StringView(StringView&& other); // Move constructor

    static StringView FromLiteral(const char* literal);

    StringView From(int start) const;
    StringView To(int end) const;
    StringView Slice(int start, int end) const;

    int Size() const { return _end - _start; }

    const char* begin() const { return _buffer->GetData() + _start; }
    const char* end() const { return _buffer->GetData() + _end; }

    StringView& operator=(const StringView& other); // Assignment operator
    bool operator==(const StringView& other) const;
    bool operator<(const StringView& other) const;
    char operator[](int index) const;
    explicit operator std::string() const;

private:
    StringView(int start, int end, std::shared_ptr<IStringBuffer> buffer);
};

StringView operator"" _sv(const char* str, size_t);

#include <iostream>

std::ostream& operator<<(std::ostream& os, const StringView& sv);

#include <functional>

// Hash function:
namespace std
{
    template<>
    struct hash<StringView>
    {
        typedef StringView argument_type;
        typedef std::size_t result_type;
        result_type operator()(const argument_type& arg) const
        {
            hash<string> stringHash;
            return stringHash((string)arg);
        }
    };
}














