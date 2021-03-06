#include "Regex.hpp"

namespace regex
{

Regex::Regex() : m_match(), m_regex()
{
}

Regex::Regex(std::string const &pattern) : m_match(), m_regex(std::regex(pattern))
{
}

Regex::~Regex()
{
}

bool Regex::match(std::string const &line) const
{
    return (std::regex_match(line, m_regex));
}

bool Regex::search(std::string const &input, uint32_t pos)
{
    return (regex_search(input.begin() + pos, input.end(), m_match, m_regex));
}

std::string Regex::replace(std::string const &input, std::string const &replace) const
{
    return (std::regex_replace(input, m_regex, replace));
}

std::smatch const &Regex::getMatch() const
{
    return (m_match);
}

std::regex const &Regex::getRegex() const
{
    return (m_regex);
}
}