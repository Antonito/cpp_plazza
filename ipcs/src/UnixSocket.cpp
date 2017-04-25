#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "UnixSocket.hpp"
#include "Logger.hpp"
#include "CommunicationError.hpp"

constexpr size_t UnixSocket::buffSize;

UnixSocket::UnixSocket() : FileDescriptorCommunicable(), m_socks()
{
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_socks) == -1)
    {
      throw CommunicationError(
          "Failed create unix socket"); // TODO: more accurate error message?
    }
}

bool UnixSocket::write(IMessage const &m) const
{
  assert(m_writeFd != -1);
  if (canWrite())
    {
      uint32_t size = htonl(static_cast<std::uint32_t>(m.getSize()));

      if (::write(m_writeFd, &size, sizeof(uint32_t)) != -1)
	{
	  ssize_t ret;

	  ret = ::write(m_writeFd, m.getData(), m.getSize());
	  return (ret != -1);
	}
    }
  return (false);
}

bool UnixSocket::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      uint32_t size;

      if (::read(m_readFd, &size, sizeof(uint32_t)) > 0)
	{
	  size = ntohl(size);
	  assert(size < UnixSocket::buffSize);

	  std::unique_ptr<uint8_t[]> buff =
	      std::make_unique<uint8_t[]>(UnixSocket::buffSize);
	  ssize_t ret;

	  ret = ::read(m_readFd, buff.get(), size);
	  if (ret <= 0)
	    {
	      return (false);
	    }
	  m.setData(static_cast<size_t>(ret), std::move(buff));
	  return (true);
	}
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
  nope::log::Log(Debug) << "Configured client";
}

void UnixSocket::configureHost()
{
  close(m_socks[SOCK_CLIENT]);
  m_socks[SOCK_CLIENT] = -1;
  m_readFd = m_socks[SOCK_HOST];
  m_writeFd = m_socks[SOCK_HOST];
  nope::log::Log(Debug) << "Configured host";
}
