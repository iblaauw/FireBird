#pragma once

#include "message_exception.h"
#include <string>

namespace utils
{
    DECL_EXCEPTION(StringParseException);

    unsigned int ParseUnsignedInt(const std::string& str);
}
