#ifndef ISERIALIZABLE_HPP_
#define ISERIALIZABLE_HPP_

class ISerializable
{
public:
  virtual ~ISerializable() = 0;
  virtual std::unique_ptr<uint8_t[]> serialize(std::size_t &sizeToFill) = 0;
  virtual void deserialize(std::size_t size, uint32_t *data) = 0;
};

#endif // !ISERIALIZABLE_HPP_