#ifndef REGEX_HPP_
#define REGEX_HPP_

#include <string>
#include <iostream>

#if __cplusplus < 201103L
#include <regex.h>
#else
#include <regex>
#endif

namespace regex
{
class Regex
{
public:
  Regex(){};
  ~Regex();

#if __cplusplus >= 201103L
  Regex(Regex const &) = delete;
  Regex &operator=(Regex const &) = delete;
#endif

  bool match(std::string const &, std::string const &);
  bool search(std::string &, std::string const &);

private:
  std::smatch m_match;
};
}

#endif // !REGEX_HPP_