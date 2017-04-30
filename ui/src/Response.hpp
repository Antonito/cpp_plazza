#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <vector>
#include "ISerializable.hpp"

class Response : public ISerializable
{
public:
  Response();
  Response(bool availability, std::uint32_t busy, std::uint32_t waiting);
  Response(Response const &other);
  virtual ~Response();

  Response &operator=(Response const &other);

  virtual std::unique_ptr<std::uint8_t[]> serialize(std::size_t &sizeToFill) const;
  virtual void deserialize(std::size_t size, std::uint8_t *data);

  bool isAvailable() const;
  std::uint32_t getBusy() const;
  std::uint32_t getWaiting() const;

private:
  std::uint8_t  m_available;
  std::uint32_t m_busy;
  std::uint32_t m_waiting;
};

#endif // !RESPONSE_HPP_