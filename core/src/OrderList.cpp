#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "OrderList.hpp"
#include "SerializerError.hpp"

OrderList::OrderList() : m_files(), m_info(Information::PHONE_NUMBER)
{
}

OrderList::OrderList(OrderList const &other)
    : m_files(other.m_files), m_info(other.m_info)
{
}

OrderList::~OrderList()
{
}

OrderList &OrderList::operator=(OrderList const &other)
{
  if (this != &other)
    {
      m_files = other.m_files;
      m_info = other.m_info;
    }
  return (*this);
}

std::string &OrderList::operator[](size_t n)
{
  return (m_files[n]);
}

size_t OrderList::size() const
{
  return (m_files.size());
}

void OrderList::addFile(std::string const &file)
{
  m_files.push_back(file);
}

void OrderList::setInfo(Information info)
{
  m_info = info;
}

Information OrderList::getInfo() const
{
  return (m_info);
}

Order OrderList::getOrder(size_t n) const
{
  return (Order(m_files[n], m_info));
}

bool OrderList::parse(OrderList &OrderList, std::stringstream &input)
{
  std::string              curWord;
  char                     c;
  char                     lastChar = ' ';
  static const std::string info[] = {"PHONE_NUMBER", "EMAIL_ADDRESS",
                                     "IP_ADDRESS"};
  int addedFiles = 0;

  OrderList.m_files.clear();

  while (input.get(c) && c != ';')
    {
      if ((lastChar == ' ' || lastChar == '\t') && (c != ' ' && c != '\t') &&
          curWord.length() != 0)
	{
	  OrderList.m_files.push_back(std::move(curWord));
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
	  OrderList.m_info = static_cast<Information>(i);
	  break;
	}
    }

  if (i == sizeof(info) / sizeof(info[0]))
    {
      throw std::invalid_argument("'" + curWord +
                                  "' is not a valid information type");
    }
  return (true);
}