#include <arpa/inet.h>
#include <cstring>
#include <exception>
#include "Response.hpp"
#include "SerializerError.hpp"
#include "Logger.hpp"

Response::Response() : m_available(2), m_infos()
{
}

Response::Response(bool availability) : m_available(availability), m_infos()
{
}

Response::Response(Response const &other)
    : m_available(other.m_available), m_infos(other.m_infos)
{
}

Response::~Response()
{
}

Response &Response::operator=(Response const &other)
{
  if (this == &other)
    return (*this);
  m_available = other.m_available;
  m_infos = other.m_infos;
  return (*this);
}

std::unique_ptr<uint8_t[]> Response::serialize(size_t &sizeToFill) const
{
  uint32_t toSend;

  sizeToFill = 0;
  // Number of process
  sizeToFill += static_cast<uint32_t>(sizeof(uint32_t));
  sizeToFill += static_cast<uint32_t>(sizeof(uint8_t)); // Availability state
  sizeToFill += m_infos.size() * sizeof(Info);

  std::unique_ptr<uint8_t[]> serial(new uint8_t[sizeToFill]);
  size_t                     cursor = 0;

  toSend = htonl(static_cast<uint32_t>(m_infos.size() + sizeof(uint8_t)));
  std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
  cursor += sizeof(uint32_t);
  std::memcpy(&serial[cursor], &m_available, sizeof(uint8_t));
  cursor += sizeof(uint8_t);

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
      nope::log::Log(Error) << "Err ! Size: " << size
                            << " {Response Deserialize}";
      throw SerializerError("Response: Not enough data to deserialize {1}");
    }

  std::memcpy(&received, &data[cursor], sizeof(uint32_t));
  cursor += sizeof(uint32_t);
  std::memcpy(&m_available, &data[cursor], sizeof(uint8_t));
  cursor += sizeof(uint8_t);
  processNb = ntohl(received) - static_cast<uint32_t>(sizeof(uint8_t));

  for (size_t i = 0; i < processNb; ++i)
    {
      if (size - cursor < sizeof(Info))
	{
	  throw SerializerError(
	      "Response: Not enough data to deserialize {2}");
	}
      std::memcpy(&tmp, &data[cursor], sizeof(Info));
      cursor += sizeof(Info);
      m_infos.push_back({ntohl(tmp.busy), ntohl(tmp.waiting),
                         ntohl(tmp.processing), ntohl(tmp.processed)});
    }
}

bool Response::isAvailable() const
{
  return (m_available == 1);
}
