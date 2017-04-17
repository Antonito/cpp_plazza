#ifndef IMESSAGE_HPP_
#define IMESSAGE_HPP_

#include <cstdint>

class IMessage
{
public:
  virtual ~IMessage(){};

  virtual size_t         length() const = 0;
  virtual uint8_t const *getData() const = 0;
  virtual void fill(uint8_t const *data, size_t size) = 0;
};

#endif // !IMESSAGE_HPP_
