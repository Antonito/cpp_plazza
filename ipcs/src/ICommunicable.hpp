#ifndef ICOMMUNICABLE_HPP_
#define ICOMMUNICABLE_HPP_

#include "IMessage.hpp"

class ICommunicable
{
public:
  virtual ~ICommunicable(){};

  virtual bool write(IMessage const &);
  virtual bool read(IMessage &);
};

#endif // !ICOMMUNICABLE_HPP_
