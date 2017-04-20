#include "AException.hpp"

AException::AException(std::string const &s) noexcept : m_message(s)
{
}

AException::AException(AException const &other) noexcept
    : m_message(other.m_message)
{
}

AException::~AException()
{
}

AException &AException::operator=(AException const &other)
{
  if (this != &other)
    {
      m_message = other.m_message;
    }
  return (*this);
}

const char *AException::what() const noexcept
{
  return (m_message.c_str());
}
