#include <cassert>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "Pipe.hpp"

constexpr size_t Pipe::buffSize;

Pipe::Pipe() : FileDescriptorCommunicable(), m_socks()
{
  if (pipe(m_socks) == -1)
    {
      throw std::exception(); // TODO
    }
}

bool Pipe::write(IMessage const &m)
{
  assert(m_writeFd != -1);
  if (canWrite())
    {
      ssize_t ret;

      ret = ::write(m_writeFd, m.getData(), m.getSize());
      return (ret != -1);
    }
  return (false);
}

bool Pipe::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      std::unique_ptr<uint8_t[]> buff =
          std::make_unique<uint8_t[]>(Pipe::buffSize);
      ssize_t ret;

      ret = ::read(m_readFd, buff.get(), Pipe::buffSize - 1);
      if (ret == -1)
	{
	  return (false);
	}
      m.setData(static_cast<size_t>(ret), std::move(buff));
      return (true);
    }
  return (false);
}

void Pipe::configureClient()
{
  close(m_socks[SOCK_HOST]);
  m_socks[SOCK_HOST] = -1;
  m_readFd = SOCK_CLIENT;
  m_writeFd = SOCK_CLIENT;
}

void Pipe::configureHost()
{
  close(m_socks[SOCK_CLIENT]);
  m_socks[SOCK_CLIENT] = -1;
  m_readFd = SOCK_HOST;
  m_writeFd = SOCK_HOST;
}