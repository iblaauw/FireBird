#pragma once

#include <string>
#include <vector>

namespace patch
{
    std::vector<std::string>& split_ex(const std::string& str, char delim, std::vector<std::string>& items);

    std::vector<std::string> split(const std::string& str, char delim);
}
