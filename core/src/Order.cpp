#include <arpa/inet.h>
#include <cstring>
#include "Order.hpp"

Order::Order() : m_file(""), m_info(PHONE_NUMBER)
{
}

Order::Order(std::string const &file, Information info)
    : m_file(file), m_info(info)
{
}

Order::Order(Order const &other) : m_file(other.m_file), m_info(other.m_info)
{
}

Order::~Order()
{
}

Order &Order::operator=(Order const &other)
{
  if (this != &other)
    {
      m_file = other.m_file;
      m_info = other.m_info;
    }
  return (*this);
}

std::unique_ptr<uint8_t[]> Order::serialize(size_t &sizeToFill) const
{
  sizeToFill = sizeof(uint32_t) + m_file.length() +
               sizeof(int32_t); // String length + str itself + info

  std::unique_ptr<uint8_t[]> serial(new uint8_t[sizeToFill]);
  size_t                     cursor = 0;
  uint32_t                   toSend;

  toSend = htonl(static_cast<uint32_t>(m_file.length()));
  std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
  cursor += sizeof(uint32_t);

  std::memcpy(&serial[cursor], m_file.c_str(), m_file.length());
  cursor += m_file.length();

  toSend = htonl(static_cast<uint32_t>(m_info));
  std::memcpy(&serial[cursor], &toSend, sizeof(uint32_t));
  return (serial);
}

void Order::deserialize(size_t size, uint8_t *data)
{
  uint32_t tmp;
  uint32_t cursor = 0;
  uint32_t length;

  std::memcpy(&tmp, &data[cursor], sizeof(uint32_t));
  length = ntohl(tmp);
  cursor += sizeof(uint32_t);

  m_file = std::string(reinterpret_cast<char *>(&data[cursor]), length);
  cursor += length;

  std::memcpy(&tmp, &data[cursor], sizeof(uint32_t));
  m_info = static_cast<Information>(ntohl(tmp));
}

std::string const &Order::getFile() const
{
  return (m_file);
}

Information Order::getInfo() const
{
  return (m_info);
}
