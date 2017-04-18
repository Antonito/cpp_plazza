#include "Order.hpp"

Order::Order()
{
}

Order::Order(Order const &other)
{
}

Order::~Order()
{
}

Order &Order::operator=(Order const &other)
{
  if (this != &other)
  {
  }
  return (*this);
}

std::string &Order::operator[](std::size_t n)
{
  return (m_files[n]);
}

std::size_t Order::size() const
{
  return (m_files.size());
}

void Order::addFile(std::string const &file)
{
  m_files.push_back(file);
}

void Order::setInfo(Information info)
{
  m_info = info;
}

Information Order::getInfo() const
{
  return (m_info);
}

std::unique_ptr<uint8_t[]> Order::serialize(int32_t &sizeToFill)
{
  sizeToFill = 1;
  return (std::make_unique<uint8_t[]>(1));
}

void Order::deserialize(int32_t size, uint32_t *data)
{

}