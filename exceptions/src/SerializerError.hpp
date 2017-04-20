#ifndef SERIALIZERERROR_HPP_
#define SERIALIZERERROR_HPP_

#include "AException.hpp"

class SerializerError : public AException
{
public:
  SerializerError(std::string const &msg = "") noexcept;
  virtual ~SerializerError();
};

#endif // !SERIALIZERERROR_HPP_