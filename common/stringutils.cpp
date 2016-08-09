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

        std::cout << "correct: " << (void*)correctEnd << std::endl;
        std::cout << "produced: " << (void*)endptr << std::endl;


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

