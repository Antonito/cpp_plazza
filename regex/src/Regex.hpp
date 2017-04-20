#ifndef REGEX_HPP_
#define REGEX_HPP_

#include <string>
#include <iostream>
#include <regex>

namespace regex
{
class Regex
{
public:
  Regex();
  Regex(std::string const &);
  ~Regex();

#if __cplusplus >= 201103L
  Regex(Regex const &) = delete;
  Regex &operator=(Regex const &) = default;
#endif

  bool match(std::string const &) const;
  bool search(std::string &);
  std::string replace(std::string const &, std::string const &) const;

private:
  std::smatch m_match;
  std::regex m_regex;
};
}

#endif // !REGEX_HPP_