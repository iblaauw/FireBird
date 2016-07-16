#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

#define FIRE_DEBUG

typedef uint8_t byte;
typedef uint32_t word;

namespace patch
{
    // Fixes the fact that my compiler has no to_string (known bug)
    template<typename T>
    std::string to_string(const T& val)
    {
        std::ostringstream ss;
        ss << val;
        return ss.str();
    }
}

#define DEBUG_TRACE \
do { \
    std::cout << "TRACE " << __FILE__ << " in " << __func__ << " at line " << __LINE__ << std::endl; \
} while (false)


