#ifndef COMMUNICATIONERROR_HPP_
#define COMMUNICATIONERROR_HPP_

#include "AException.hpp"

class CommunicationError : public AException
{
public:
  CommunicationError(std::string const &msg = "") noexcept;
  virtual ~CommunicationError();
};

#endif // !COMMUNICATIONERROR_HPP_