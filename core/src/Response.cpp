#include <arpa/inet.h>
#include <cstring>
#include "Response.hpp"

Response::Response() : m_infos()
{
}

Response::Response(Response const &other) : m_infos(other.m_infos)
{
}

Response::~Response()
{
}

Response &Response::operator=(Response const &other)
{
  if (this == &other)
    return (*this);
  m_infos = other.m_infos;
  return (*this);
}

std::unique_ptr<uint8_t[]> Response::serialize(size_t &sizeToFill)
{
  uint32_t toSend;

  // Number of process
  sizeToFill += static_cast<uint32_t>(sizeof(uint32_t));
  for (std::vector<bool> const &v : m_infos)
    {
      // Number of thread + status of thoses threads
      sizeToFill +=
          static_cast<uint32_t>(sizeof(uint32_t) + v.size() * sizeof(uint8_t));
    }

  std::unique_ptr<uint8_t[]> serial(new uint8_t[sizeToFill]);
  size_t                     cursor = 0;

  toSend = htonl(static_cast<uint32_t>(m_infos.size()));
  std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
  cursor += sizeof(uint32_t);

  for (std::vector<bool> const &v : m_infos)
    {
      toSend = htonl(static_cast<uint32_t>(v.size()));
      std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
      cursor += sizeof(uint32_t);
      for (bool b : v)
	{
	  serial[cursor] = static_cast<uint8_t>(b);
	  cursor += sizeof(uint8_t);
	}
    }
  return (serial);
}

void Response::deserialize(size_t size, uint8_t *data)
{
  size_t   cursor = 0;
  uint32_t received;
  uint32_t processNb;

  if (size - cursor < sizeof(uint32_t))
    {
      throw SerializerError("Not enough data to deserialize");
    }

  std::memcpy(&received, &data[cursor], sizeof(uint32_t));
  cursor += sizeof(uint32_t);
  processNb = ntohl(received);
  m_infos.insert(m_infos.begin(), processNb, std::vector<bool>());

  for (uint32_t i = 0; i < processNb; ++i)
    {
      uint32_t count;

      if (size - cursor < sizeof(uint32_t))
	{
	  throw SerializerError("Not enough data to deserialize");
	}
      std::memcpy(&received, &data[cursor], sizeof(uint32_t));
      cursor += sizeof(uint32_t);
      count = ntohl(received);
      m_infos[i].insert(m_infos[i].begin(), count, false);
      for (uint32_t j = 0; j < count; ++j)
	{
	  m_infos[i][j] = static_cast<bool>(data[cursor]);
	  cursor += sizeof(uint8_t);
	}
    }
}