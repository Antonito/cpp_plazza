#include <limits>
#include <iostream>
#include <fstream>
#include "Worker.hpp"

Worker::Worker() : m_reg(), m_data(), m_uncipherData(), m_result()
{
}

Worker::~Worker()
{
}

void Worker::exec(Order &order)
{
  setReg(order.getInfo());
  loadFile(order.getFile());
  uncipher();
}

void Worker::setReg(Information info)
{
  static std::vector<std::string> regInfo = {
      "([0-9] ?){10}",
      "[a-zA-Z0-9_.-]+ '@' [a-zA-Z0-9_.-]+", // magic quote ?
      "[0-255].[0-255].[0-255].[0-255]",
  };
  m_reg = regex::Regex(regInfo[static_cast<int>(info)]);
}

void Worker::loadFile(std::string const &fileName)
{
  std::stringstream tmp;
  std::ifstream     file;

  file.open(fileName.c_str());
  if (file.is_open())
    {
      tmp << file.rdbuf();
      m_data = tmp.str();
      m_uncipherData = tmp.str();
    }
  // std::cout << m_uncipherData << std::endl;
}

void Worker::uncipher()
{
  // Check if file is not ciphered
  if (m_reg.search(m_uncipherData))
    {
      fillResult();
      return;
    }
  std::cout << "NO MATCH" << std::endl;
  // Bruteforce Xor
  for (int8_t i = 0; i < std::numeric_limits<int8_t>::max(); i++)
    {
      if (uncipherXor(i))
	{
	  fillResult();
	  return;
	}
    }

  // Bruteforce Caesar
  for (int16_t i = 0; i < std::numeric_limits<int16_t>::max(); i++)
    {
      if (uncipherCaesar(i))
	{
	  fillResult();
	  return;
	}
    }
}

void Worker::fillResult()
{
  int32_t searchPosition = 0;

  while (m_reg.search(m_uncipherData, searchPosition))
    {
      std::smatch const &match = m_reg.getMatch();

      std::cout << match[0] << std::endl;
      searchPosition += match.prefix().length() + match[0].length();
    }
}

bool Worker::uncipherXor(uint8_t key)
{
  for (int32_t i = 0; i < static_cast<int32_t>(m_data.size()); i++)
    m_uncipherData[i] = static_cast<char>(m_data[i] ^ key);
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}

bool Worker::uncipherCaesar(uint16_t key)
{
  for (int32_t i = 0; i < static_cast<int32_t>(m_data.size()); i++)
    {
      if (static_cast<uint16_t>(m_data[i]) >= key)
	m_uncipherData[i] = static_cast<char>((m_data[i] - key - 'A') % 26);
      else
	m_uncipherData[i] =
	    static_cast<char>((m_data[i] + 26 - (key - 'A')) % 26);
    }
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}