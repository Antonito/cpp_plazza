#include <arpa/inet.h>
#include <cstring>
#include <exception>
#include "Response.hpp"
#include "SerializerError.hpp"
#include "Logger.hpp"

Response::Response() : m_available(2), m_busy(0), m_waiting(0)
{
}

Response::Response(bool availability, std::uint32_t busy,
                   std::uint32_t waiting)
    : m_available(availability), m_busy(busy), m_waiting(waiting)
{
}

Response::Response(Response const &other)
    : m_available(other.m_available), m_busy(other.m_busy),
      m_waiting(other.m_waiting)
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
  m_busy = other.m_busy;
  m_waiting = other.m_waiting;
  return (*this);
}

std::unique_ptr<std::uint8_t[]>
    Response::serialize(std::size_t &sizeToFill) const
{
  std::uint32_t toSend;

  sizeToFill = 0;
  sizeToFill += static_cast<std::uint32_t>(
      sizeof(std::uint8_t) + 2 * sizeof(std::uint32_t)); // Availability state

  std::unique_ptr<std::uint8_t[]> serial(new std::uint8_t[sizeToFill]);
  std::size_t                     cursor = 0;

  std::memcpy(&serial[cursor], &m_available, sizeof(std::uint8_t));
  cursor += sizeof(std::uint8_t);

  toSend = htonl(m_busy);
  std::memcpy(&serial[cursor], &toSend, sizeof(std::uint32_t));
  cursor += sizeof(std::uint32_t);

  toSend = htonl(m_waiting);
  std::memcpy(&serial[cursor], &toSend, sizeof(std::uint32_t));
  cursor += sizeof(std::uint32_t);
  return (serial);
}

void Response::deserialize(std::size_t size, std::uint8_t *data)
{
  std::size_t   cursor = 0;
  std::uint32_t received;

  if (size - cursor < 2 * sizeof(std::uint32_t) + sizeof(std::uint8_t))
    {
      nope::log::Log(Error)
          << "Err ! Size: " << size << " {Response Deserialize}";
      throw SerializerError("Response: Not enough data to deserialize {1}");
    }

  std::memcpy(&m_available, &data[cursor], sizeof(std::uint8_t));
  cursor += sizeof(std::uint8_t);

  std::memcpy(&received, &data[cursor], sizeof(std::uint32_t));
  m_busy = ntohl(received);
  cursor += sizeof(std::uint32_t);

  std::memcpy(&received, &data[cursor], sizeof(std::uint32_t));
  m_waiting = ntohl(received);
  cursor += sizeof(std::uint32_t);
}

bool Response::isAvailable() const
{
  return (m_available == 1);
}

std::uint32_t Response::getBusy() const
{
  return (m_busy);
}

std::uint32_t Response::getWaiting() const
{
  return (m_waiting);
}
