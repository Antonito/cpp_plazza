#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "UnixSocket.hpp"

constexpr size_t UnixSocket::buffSize;

UnixSocket::UnixSocket() : FileDescriptorCommunicable(), m_socks()
{
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_socks) == -1)
    {
      throw std::exception(); // TODO
    }
}

bool UnixSocket::write(IMessage const &m)
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

bool UnixSocket::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      std::unique_ptr<uint8_t[]> buff =
          std::make_unique<uint8_t[]>(UnixSocket::buffSize);
      ssize_t ret;

      ret = ::read(m_readFd, buff.get(), UnixSocket::buffSize - 1);
      if (ret == -1)
	{
	  return (false);
	}
      m.setData(static_cast<size_t>(ret), std::move(buff));
      return (true);
    }
  return (false);
}

void UnixSocket::configureClient()
{
  close(m_socks[SOCK_HOST]);
  m_socks[SOCK_HOST] = -1;
  m_readFd = m_socks[SOCK_CLIENT];
  m_writeFd = m_socks[SOCK_CLIENT];
  toggleTimeout();
}

void UnixSocket::configureHost()
{
  close(m_socks[SOCK_CLIENT]);
  m_socks[SOCK_CLIENT] = -1;
  m_readFd = m_socks[SOCK_HOST];
  m_writeFd = m_socks[SOCK_HOST];
}