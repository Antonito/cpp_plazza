#ifndef AEXCEPTION_HPP_
#define AEXCEPTION_HPP_

#include <exception>
#include <string>

class AException : public std::exception
{
public:
  AException(std::string const &s = "") noexcept;
  AException(AException const &other) noexcept;
  virtual ~AException() = 0;

  AException &operator=(AException const &other);

  virtual const char *what() const noexcept;

private:
  std::string m_message;
};

#endif // !AEXCEPTION_HPP_
