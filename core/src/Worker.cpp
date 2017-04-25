#include <limits>
#include <iostream>
#include <fstream>
#include "Worker.hpp"
#include "Logger.hpp"

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
      "0[0-9]([ ]?[0-9]{2}){4}",
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
  uint8_t buf[2];
  nope::log::Log(Info) << "Trying to XOR bruteforce";
  for (uint16_t i = 0; i < std::numeric_limits<uint16_t>::max(); i++)
    {
      constexpr uint8_t max = std::numeric_limits<uint8_t>::max();

      buf[1] = static_cast<uint8_t>(i % max);
      buf[0] = static_cast<uint8_t>(i / max);
      if (uncipherXor(buf, (i < max) ? 1 : 2))
	{
	  fillResult();
	  return;
	}
    }

  // Bruteforce Caesar
  nope::log::Log(Info) << "Trying to Caesar bruteforce";
  for (uint8_t i = 0; i < std::numeric_limits<uint8_t>::max(); i++)
    {
      if (uncipherCaesar(i))
	{
	  fillResult();
	  return;
	}
    }
  nope::log::Log(Warning) << "Didn't find any result";
}

void Worker::fillResult()
{
  uint32_t searchPosition = 0;

  while (m_reg.search(m_uncipherData, searchPosition))
    {
      std::smatch const &match = m_reg.getMatch();
      std::cout << match[0] << std::endl;
      nope::log::Log(Info) << "Found : " << match[0];
      searchPosition +=
          static_cast<uint32_t>(match.prefix().length() + match[0].length());
    }
}

bool Worker::uncipherXor(uint8_t *key, size_t len)
{
  for (uint32_t i = 0; i < m_data.size(); i++)
    m_uncipherData[i] = static_cast<char>(m_data[i] ^ key[i % len]);
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}

bool Worker::uncipherCaesar(uint8_t key)
{
  int tmp;

  for (uint32_t i = 0; i < m_data.size(); i++)
    {
      tmp = m_data[i] - key;
      if (tmp < 0)
	{
	  tmp += 127;
	}
      m_uncipherData[i] = static_cast<char>(tmp);
    }
  if (m_reg.search(m_uncipherData))
    return true;
  return false;
}