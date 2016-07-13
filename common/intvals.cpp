#include "intvals.h"

std::ostream& operator<<(std::ostream& os, const uint8_t& i)
{
    return os << static_cast<unsigned int>(i);
}

std::ostream& operator<<(std::ostream& os, const uint16_t& i)
{
    return os << static_cast<unsigned int>(i);
}
