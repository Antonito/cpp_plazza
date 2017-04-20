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
  // First vector represent process
  // Second one represent each thread
  // The bool indicate if the thread is busy
  std::vector<std::vector<bool>> m_infos;
};

#endif // !RESPONSE_HPP_