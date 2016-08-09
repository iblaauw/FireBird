#include "intvals.h"

std::ostream& operator<<(std::ostream& os, const uint8_t& i)
{
    return os << static_cast<unsigned int>(i);
}

std::ostream& operator<<(std::ostream& os, const uint16_t& i)
{
    return os << static_cast<unsigned int>(i);
}

#include <limits>

static_assert(std::numeric_limits<float>::is_iec559, "Error, this compiler doesn't use IEEE 754 standard for float.");
static_assert(std::numeric_limits<double>::is_iec559, "Error, this compiler doesn't use IEEE 754 standard for double.");

