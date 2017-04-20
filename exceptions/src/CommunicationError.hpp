#ifndef COMMUNICATIONERROR_HPP_
#define COMMUNICATIONERROR_HPP_

#include "AException.hpp"

class CommunicationError : public AException
{
public:
  CommunicationError(std::string const &msg = "");
  virtual ~CommunicationError();
};

#endif // !COMMUNICATIONERROR_HPP_