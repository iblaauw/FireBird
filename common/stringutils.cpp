#include "stringutils.h"
#include <cstdint>
#include <sstream>
#include <cstdlib>

#include <iostream>

#define _SIZE_LEN_PROXY(MAX_SIZE) std::string( #MAX_SIZE ).size()
#define SIZE_LEN(MAX_SIZE) _SIZE_LEN_PROXY(MAX_SIZE)

namespace utils
{
    static bool isNum(char c)
    {
        return c >= '0' && c <= '9';
    }

    static void AssertAllDigits(const std::string& str, const std::string& message)
    {
        for (char c : str)
        {
            if (!isNum(c))
                throw StringParseException(message);
        }
    }

    uint32_t ParseUnsignedInt(const std::string& str)
    {
        for (char c : str)
        {
            if (!isNum(c))
                throw StringParseException("Invalid Number");
        }

        size_t maxlen = SIZE_LEN(UINT32_MAX);

        if (str.size() > maxlen)
            throw StringParseException("Out of Range");

        uint32_t val = 0;
        uint32_t cache = 0;
        for (char c : str)
        {
            cache = val;

            uint32_t toadd = c - '0';

            val *= 10;
            val += toadd;

            if (val < cache) // Overflow
                throw StringParseException("Out of Range");
        }

        return val;
    }

    template<class T>
    static T ParseUIntType(const std::string& str, size_t maxlen)
    {
        AssertAllDigits(str, "Invalid Number");

        if (str.size() > maxlen)
            throw StringParseException("Out of Range");

        T val = 0;
        T cache = 0;

        for (char c : str)
        {
            cache = val;

            T toadd = c - '0';

            val *= 10;
            val += toadd;

            if (val < cache) // Overflow
                throw StringParseException("Out of Range");
        }

        return val;
    }

    uint32_t ParseUint32(const std::string& str)
    {
        size_t maxlen = SIZE_LEN(UINT32_MAX);
        return ParseUIntType<uint32_t>(str, maxlen);
    }

    uint16_t ParseUint16(const std::string& str)
    {
        size_t maxlen = SIZE_LEN(UINT16_MAX);
        return ParseUIntType<uint16_t>(str, maxlen);
    }

    static bool IsHexChar(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' || c <= 'F');
    }


    static uint8_t HexCharValue(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';

        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;

        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        throw StringParseException("Invalid hex character");
    }

    bool IsHexStr(const std::string& str)
    {
        if (str.size() < 3)
            return false;

        if (str[0] != '0')
            return false;

        if (str[1] != 'x')
            return false;

        for (size_t i = 2; i < str.size(); i++)
        {
            if (!IsHexChar(str[i]))
                return false;
        }

        return true;
    }

    template<class T>
    static T ParseUHexType(const std::string& str)
    {
        if (!IsHexStr(str))
            throw StringParseException("Invalid Hex String");

        T val = 0;
        size_t numBits = sizeof(T) * 8;
        T maxval = (1 << (numBits - 4));

        for (size_t i = 2; i < str.size(); i++)
        {
            if (val >= maxval)
                throw StringParseException("Out of Range");

            uint8_t cval = HexCharValue(str[i]);

            val *= 16;
            val += cval;
        }

        return val;
    }

    uint32_t ParseHexUint32(const std::string& str) { return ParseUHexType<uint32_t>(str); }

    uint16_t ParseHexUint16(const std::string& str) { return ParseUHexType<uint16_t>(str); }

    static uint32_t GetMantissa(const std::string& str)
    {
        AssertAllDigits(str, "Invalid Number");

        uint64_t base = 1;
        uint64_t accumulated = 0;
        uint32_t val = 0;
        int count = 0;

        for (char c : str)
        {
            if (count >= 23)
                throw StringParseException("Out of Range");
            base *= 5;
            val = val << 1;
            accumulated *= 10;

            int charVal = c - '0';
            accumulated += charVal;
            if (accumulated > base)
            {
                accumulated -= base;
                val += 1;
            }

            count++;
        }

        return val;
    }

    bool TryParseFloat(const std::string& str, float* outVal)
    {
        if (str.empty())
            throw MessageException("Invalid Argument");

        char* endptr;
        float val = strtof(str.c_str(), &endptr);

        const char* correctEnd = str.c_str() + str.size();

        if (endptr != correctEnd)
            return false;

        *outVal = val;
        return true;
    }

    std::vector<std::string> Split(const std::string& str, char delim, bool blanks)
    {
        std::vector<std::string> items;
        return SplitEx(str, delim, items, blanks);
    }

    std::vector<std::string>& SplitEx(const std::string& str, char delim, std::vector<std::string>& items, bool blanks)
    {
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            if (!item.empty() || blanks)
                items.push_back(item);
        }

        return items;
    }

}

