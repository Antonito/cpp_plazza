#include <arpa/inet.h>
#include <cstring>
#include <exception>
#include "Response.hpp"
#include "SerializerError.hpp"

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

  sizeToFill = 0;
  // Number of process
  sizeToFill += static_cast<uint32_t>(sizeof(uint32_t));
  sizeToFill += m_infos.size() * sizeof(Info);

  std::unique_ptr<uint8_t[]> serial(new uint8_t[sizeToFill]);
  size_t                     cursor = 0;

  toSend = htonl(static_cast<uint32_t>(m_infos.size()));
  std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
  cursor += sizeof(uint32_t);

  for (Info const &i : m_infos)
    {
      Info tmp = {htonl(i.busy), htonl(i.waiting), htonl(i.processing),
                  htonl(i.processed)};

      std::memcpy(&serial[cursor], &tmp, sizeof(Info));
      cursor += sizeof(Info);
    }
  return (serial);
}

void Response::deserialize(size_t size, uint8_t *data)
{
  size_t   cursor = 0;
  uint32_t received;
  uint32_t processNb;
  Info     tmp;

  if (size - cursor < sizeof(uint32_t))
    {
      throw SerializerError("Not enough data to deserialize");
    }

  std::memcpy(&received, &data[cursor], sizeof(uint32_t));
  cursor += sizeof(uint32_t);
  processNb = ntohl(received);

  for (size_t i = 0; i < processNb; ++i)
    {
      if (size - cursor < sizeof(Info))
	{
	  throw SerializerError("Not enough data to deserialize");
	}
      std::memcpy(&tmp, &data[cursor], sizeof(Info));
      cursor += sizeof(Info);
      m_infos.push_back({ntohl(tmp.busy), ntohl(tmp.waiting),
                         ntohl(tmp.processing), ntohl(tmp.processed)});
    }
}