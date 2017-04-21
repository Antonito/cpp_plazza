#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "InternetSocket.hpp"
#include "CommunicationError.hpp"

constexpr size_t InternetSocket::buffSize;

InternetSocket::InternetSocket() : FileDescriptorCommunicable(), m_socks()
{
  if (socketpair(AF_INET, SOCK_STREAM, 0, m_socks) == -1)
    {
      throw CommunicationError(
          "Failed to open socket"); // TODO: adjust error message
    }
}

bool InternetSocket::write(IMessage const &m) const
{
  assert(m_writeFd != -1);
  if (canWrite())
    {
      uint32_t size = htonl(m.getSize());

      if (::write(m_writeFd, &size, sizeof(uint32_t)) != -1)
	{
	  ssize_t ret;

	  ret = ::write(m_writeFd, m.getData(), m.getSize());
	  return (ret != -1);
	}
    }
  return (false);
}

bool InternetSocket::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      uint32_t size;

      if (::read(m_readFd, &size, sizeof(uint32_t)) > 0)
	{
	  size = ntohl(size);
	  assert(size < InternetSocket::buffSize);

	  std::unique_ptr<uint8_t[]> buff =
	      std::make_unique<uint8_t[]>(InternetSocket::buffSize);
	  ssize_t ret;

	  ret = ::read(m_readFd, buff.get(), size);
	  if (ret == -1)
	    {
	      return (false);
	    }
	  m.setData(static_cast<size_t>(ret), std::move(buff));
	  return (true);
	}
    }
  return (false);
}

void InternetSocket::configureClient()
{
  close(m_socks[SOCK_HOST]);
  m_socks[SOCK_HOST] = -1;
  m_readFd = m_socks[SOCK_CLIENT];
  m_writeFd = m_socks[SOCK_CLIENT];
  toggleTimeout();
}

void InternetSocket::configureHost()
{
  close(m_socks[SOCK_CLIENT]);
  m_socks[SOCK_CLIENT] = -1;
  m_readFd = m_socks[SOCK_HOST];
  m_writeFd = m_socks[SOCK_HOST];
}