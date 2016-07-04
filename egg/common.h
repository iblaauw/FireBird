#include <string>
#include <sstream>

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

