#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <vector>
#include "ISerializable.hpp"

class Response : public ISerializable
{
public:
  Response();
  Response(Response const &other);
  virtual ~Response();

  Response &operator=(Response const &other);

  virtual std::unique_ptr<uint8_t[]> serialize(size_t &sizeToFill);
  virtual void deserialize(size_t size, uint8_t *data);

private:
  struct Info
  {
    uint32_t busy;
    uint32_t waiting;
    uint32_t processing;
    uint32_t processed;
  };

  std::vector<Info> m_infos;
};

#endif // !RESPONSE_HPP_