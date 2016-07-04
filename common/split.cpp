#include "split.h"

#include <sstream>

namespace patch
{
    std::vector<std::string>& split_ex(const std::string& str, char delim, std::vector<std::string>& items)
    {
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            if (!item.empty())
                items.push_back(item);
        }

        return items;
    }

    std::vector<std::string> split(const std::string& str, char delim)
    {
        std::vector<std::string> items;
        return split_ex(str, delim, items);
    }
}
