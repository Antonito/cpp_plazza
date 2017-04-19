#include <iostream>
#include <cstring>
#include "Order.hpp"

Order::Order() : m_files(), m_info(Information::PHONE_NUMBER)
{
}

Order::Order(Order const &other) : m_files(other.m_files), m_info(other.m_info)
{
}

Order::~Order()
{
}

Order &Order::operator=(Order const &other)
{
  if (this != &other)
    {
      m_files = other.m_files;
      m_info = other.m_info;
    }
  return (*this);
}

std::string &Order::operator[](size_t n)
{
  return (m_files[n]);
}

size_t Order::size() const
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

std::unique_ptr<uint8_t[]> Order::serialize(size_t &sizeToFill) const
{
  sizeToFill = 0;

  // Size of the fileCount (number of files)
  sizeToFill += sizeof(size_t);

  // Size of each string (string_size + string itself)
  for (std::string const &s : m_files)
    {
      sizeToFill += sizeof(size_t) + s.size();
    }

  // Size of information
  sizeToFill += sizeof(Information);

  // Allocation
  std::unique_ptr<uint8_t[]> serial(new uint8_t[sizeToFill]);
  size_t                     cursor = 0;
  size_t                     fileCount = m_files.size();

  // Copy the file count
  std::memcpy(&serial[cursor], &fileCount, sizeof(size_t));
  cursor += sizeof(size_t);

  // Copy each string
  for (std::string const &s : m_files)
    {
      size_t size = s.size();

      // Size of the string
      std::memcpy(&serial[cursor], &size, sizeof(size_t));
      cursor += sizeof(size_t);
      // The string itself
      std::memcpy(&serial[cursor], s.c_str(), size);
      cursor += size;
    }

  // Copy the information
  std::memcpy(&serial[cursor], &m_info, sizeof(Information));

  return (serial);
}

void Order::deserialize(size_t size, uint8_t *data)
{
  size_t fileCount;
  size_t cursor = 0;

  // Check if data is big enough
  if (size < sizeof(size_t))
    {
      throw std::exception();
    }

  // Get the file count
  std::memcpy(&fileCount, &data[cursor], sizeof(size_t));
  cursor += sizeof(size_t);

  // Loop on each file
  for (size_t i = 0; i < fileCount; ++i)
    {
      size_t len;

      // Check if data is big enough
      if (size - cursor < sizeof(size_t))
	{
	  throw std::exception();
	}

      // Get the string length
      std::memcpy(&len, &data[cursor], sizeof(size_t));
      cursor += sizeof(size_t);

      // Check if data is big enough
      if (size - cursor < len)
	{
	  throw std::exception();
	}

      // Add the string
      m_files.emplace_back(reinterpret_cast<char *>(&data[cursor]), len);
      cursor += len;
    }

  // Check if data is big enough
  if (size - cursor < sizeof(Information))
    {
      throw std::exception();
    }

  // Get the information
  std::memcpy(&m_info, &data[cursor], sizeof(Information));
}

bool Order::parse(Order &order, std::stringstream &input)
{
  std::string                  curWord;
  char c;
  char                         lastChar = ' ';
  static const std::string info[] = {"PHONE_NUMBER", "EMAIL_ADDRESS",
                                         "IP_ADDRESS"};
  int addedFiles = 0;

  order.m_files.clear();

  while (input.get(c) && c != ';')
    {
      if ((lastChar == ' ' || lastChar == '\t') && (c != ' ' && c != '\t') && curWord.length() != 0)
	{
	  order.m_files.push_back(std::move(curWord));
	  curWord = "";
	  addedFiles++;
	}
      if (c != ' ' && c != '\t')
	{
	  curWord += c;
	}
      lastChar = c;
    }

  if (addedFiles == 0)
    {
      return (false);
    }

  size_t i;
  for (i = 0; i < sizeof(info) / sizeof(info[0]); ++i)
    {
      if (curWord == info[i])
	{
	  order.m_info = static_cast<Information>(i);
	  break;
	}
    }

  if (i == sizeof(info) / sizeof(info[0]))
    {
      throw std::exception(); // Invalid information type
    }
  return (true);
}