#ifndef IMESSAGE_HPP_
#define IMESSAGE_HPP_

#include <cstdint>
#include <memory>

class IMessage
{
public:
  virtual ~IMessage(){};
  virtual int32_t getSize() const = 0;
  virtual uint8_t *getData() = 0;
  virtual void setData(int32_t size, std::unique_ptr<uint8_t[]> data) = 0;
};

#endif // !IMESSAGE_HPP_