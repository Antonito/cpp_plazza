#ifndef ISERIALIZABLE_HPP_
#define ISERIALIZABLE_HPP_

#include <cstdint>
#include <memory>

class ISerializable
{
public:
  virtual ~ISerializable() {};
  virtual std::unique_ptr<uint8_t[]> serialize(size_t &sizeToFill) = 0;
  virtual void deserialize(size_t size, uint8_t *data) = 0;
};

#endif // !ISERIALIZABLE_HPP_