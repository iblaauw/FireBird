#include "StringView.h"
#include "StringViewException.h"

#include <cstring>

StringView::StringView(const std::string& str)
{
    _start = 0;
    _end = str.size();
    _buffer = std::make_shared<CharBuffer>(str.c_str(), str.size(), true);
}

StringView::StringView(const char* str)
{
    _start = 0;
    _end = std::strlen(str);
    _buffer = std::make_shared<CharBuffer>(str, _end, true);
}

StringView::StringView(const StringView& other) :
    _buffer(other._buffer),
    _start(other._start),
    _end(other._end)
{}

StringView::StringView(StringView&& other) :
    _buffer(std::move(other._buffer)),
    _start(std::move(other._start)),
    _end(std::move(other._end))
{}

/*private*/ StringView::StringView(int start, int end, std::shared_ptr<IStringBuffer> buffer)
    : _buffer(buffer), _start(start), _end(end)
{}

/*static*/ StringView StringView::FromLiteral(const char* literal)
{
    int len = std::strlen(literal);
    auto buffer = std::make_shared<CharBuffer>(literal, len, false);

    return StringView(0, len, buffer);
}

StringView StringView::From(int start) const
{
    return Slice(start, _end);
}

StringView StringView::To(int end) const
{
    return Slice(_start, end);
}

StringView StringView::Slice(int start, int end) const
{
    int absoluteStart = start + (start < 0 ? _end : _start);
    int absoluteEnd = end + (end < 0 ? _end : _start);

    if (absoluteStart < 0 || absoluteEnd < 0 || absoluteStart > _end || absoluteEnd > _end)
        throw StringViewException("Argument out of range");

    return  StringView(absoluteStart, absoluteEnd, _buffer);
}

StringView& StringView::operator=(const StringView& other)
{
    _start = other._start;
    _end = other._end;
    _buffer = other._buffer;
    return *this;
}

bool StringView::operator==(const StringView& other) const
{
    const char* a = this->begin();
    const char* b = other.begin();
    const char* aend = this->end();
    const char* bend = other.end();
    while (a != aend && b != bend)
    {
        if (*a != *b)
            return false;
        a++;
        b++;
    }

    if (a == aend && b == bend)
        return true;
    return false;
}

bool StringView::operator<(const StringView& other) const
{
    const char* a = this->begin();
    const char* b = other.begin();
    const char* aend = this->end();
    const char* bend = other.end();
    while (a != aend && b != bend)
    {
        if (*a < *b)
            return true;
        if (*a > *b)
            return false;
        a++;
        b++;
    }

    bool adone = a == aend;
    bool bdone = b == bend;

    if (adone && !bdone)
        return true; // a shorter string is < a longer one

    if (!adone && bdone)
        return false;

    // a == b
    return false;
}

char StringView::operator[](int index) const
{
    return begin()[index];
}

StringView::operator std::string() const
{
    return std::string(_buffer->GetData(), Size());
}

