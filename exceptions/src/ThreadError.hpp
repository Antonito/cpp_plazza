#ifndef THREADERROR_HPP_
#define THREADERROR_HPP_

#include "AException.hpp"

class ThreadError : public AException
{
public:
  ThreadError(std::string const &msg) noexcept;
  virtual ~ThreadError();
};

#endif // !THREADERROR_HPP_