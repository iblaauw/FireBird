#pragma once

#include "message_exception.h"
#include <string>
#include <vector>

namespace utils
{
    DECL_EXCEPTION(StringParseException);

    unsigned int ParseUnsignedInt(const std::string& str);
    std::vector<std::string> Split(const std::string& str, char delim, bool blanks = false);
    std::vector<std::string>& SplitEx(const std::string& str, char delim, std::vector<std::string>& items, bool blanks);
}
