#pragma once

#include "message_exception.h"
#include <string>
#include <vector>

namespace utils
{
    DECL_EXCEPTION(StringParseException);

    unsigned int ParseUnsignedInt(const std::string& str);
    bool TryParseFloat(const std::string& str, float* outVal);

    std::vector<std::string> Split(const std::string& str, char delim, bool blanks = false);

    std::vector<std::string>& SplitEx(const std::string& str, char delim, std::vector<std::string>& items, bool blanks);

    uint32_t ParseUint32(const std::string& str);
    uint16_t ParseUint16(const std::string& str);

    bool IsHexStr(const std::string& str);
    uint32_t ParseHexUint32(const std::string& str);
    uint16_t ParseHexUint16(const std::string& str);


}
