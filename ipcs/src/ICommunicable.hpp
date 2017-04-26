#ifndef ICOMMUNICABLE_HPP_
#define ICOMMUNICABLE_HPP_

#include "IMessage.hpp"

class ICommunicable
{
public:
  virtual ~ICommunicable(){};

  virtual bool write(IMessage const &) const = 0;
  virtual bool read(IMessage &) = 0;

  virtual void configureClient() = 0;
  virtual void configureHost() = 0;

  virtual bool canWrite(bool const blocking = false) const = 0;
  virtual bool canRead(bool const blocking = false) const = 0;

  virtual int getWriteHandler() const = 0;
  virtual int getReadHandler() const = 0;

  virtual void close() = 0;
};

#endif // !ICOMMUNICABLE_HPP_
