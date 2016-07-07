#include "stringutils.h"
#include <cstdint>
#include <sstream>


#define _SIZE_LEN_PROXY(MAX_SIZE) std::string( #MAX_SIZE ).size()
#define SIZE_LEN(MAX_SIZE) _SIZE_LEN_PROXY(MAX_SIZE)

namespace utils
{
    static bool isNum(char c)
    {
        return c >= '0' && c <= '9';
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

