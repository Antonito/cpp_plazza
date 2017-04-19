#include "Regex.hpp"

namespace regex
{
bool Regex::match(std::string const &line, std::string const &pattern)
{

    // #if __cplusplus < 201103L

    //     regex_t reg;
    //     int ret;

    //     if (regcomp(&reg, pattern.c_str(), REG_EXTENDED) != 0)
    //         return false;
    //     ret = regexec(&re, line.c_str(), 0, NULL, 0);
    //     if (ret == 0)
    //         return true;
    //     return false;

    // #else

    return (std::regex_match(line, std::regex(pattern)));
}

bool Regex::search(std::string &input, std::string const &pattern)
{
    if (regex_search(input, m_match, std::regex(pattern)))
        return (false);
    input = m_match.suffix();
    return (true);
}
}