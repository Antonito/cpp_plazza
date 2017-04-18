#ifndef ISERIALIZABLE_HPP_
#define ISERIALIZABLE_HPP_

#include <cstdint>
#include <memory>

class ISerializable
{
public:
  virtual ~ISerializable() {};
  virtual std::unique_ptr<uint8_t[]> serialize(int32_t &sizeToFill) = 0;
  virtual void deserialize(int32_t size, uint32_t *data) = 0;
};

#endif // !ISERIALIZABLE_HPP_