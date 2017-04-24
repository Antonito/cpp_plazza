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

void Worker::exec(Order const &order)
{
  setReg(order.getInfo());
  loadFile(order.getFile());
  uncipher();
}

void Worker::setReg(Information info)
{
  static std::vector<std::string> regInfo = {
      "([0-9] ?){10}",
      "[a-zA-Z0-9_\\.-]+@[a-zA-Z0-9_\\/-]+\\.[a-zA-Z]{2,4}", // magic quote ?
      "((25[0-5]|2[0-4][0-9]|[01]?[0-9]?[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|[01]"
      "?[0-9]?[0-9])",
  };
  m_reg = regex::Regex(regInfo[info]);
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
  // Bruteforce Xor
  for (uint16_t i = 0; i < std::numeric_limits<uint16_t>::max(); i++)
    {
      if (uncipherXor(i))
	{
	  fillResult();
	  return;
	}
    }

  // Bruteforce Caesar
  for (uint8_t i = 0; i < std::numeric_limits<uint8_t>::max(); i++)
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
  uint32_t searchPosition = 0;

  while (m_reg.search(m_uncipherData, searchPosition))
    {
      std::smatch const &match = m_reg.getMatch();
      std::cout << match[0] << std::endl;
      searchPosition +=
          static_cast<uint32_t>(match.prefix().length() + match[0].length());
    }
}

bool Worker::uncipherXor(uint16_t key)
{
  for (uint32_t i = 0; i < m_data.size(); i++)
    m_uncipherData[i] = static_cast<char>(m_data[i] ^ key);
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}

bool Worker::uncipherCaesar(uint8_t key)
{
  for (uint32_t i = 0; i < m_data.size(); i++)
    {
      if (static_cast<uint8_t>(m_data[i]) >= key)
	m_uncipherData[i] = static_cast<char>((m_data[i] - key) % 127);
      else
	m_uncipherData[i] = static_cast<char>((m_data[i] + 127 - key) % 127);
    }
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}