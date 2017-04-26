#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include "InternetSocket.hpp"
#include "Logger.hpp"
#include "CommunicationError.hpp"

constexpr size_t InternetSocket::buffSize;
std::uint16_t    InternetSocket::portOff = 0;

InternetSocket::InternetSocket()
    : FileDescriptorCommunicable(), m_sock(-1), m_cliSock(-1),
      m_port(static_cast<std::uint16_t>(12345 + InternetSocket::portOff)),
      m_isHost(false)
{
  ++InternetSocket::portOff;
}

InternetSocket::~InternetSocket()
{
  if (m_isHost)
    {
      ::close(m_sock);
      m_sock = -1;
    }
}

bool InternetSocket::write(IMessage const &m) const
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

void InternetSocket::configureClient()
{
  struct sockaddr_in sin;

  m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
  if (m_sock == -1)
    {
      throw CommunicationError("Failed to create socket");
    }
  sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  sin.sin_port = htons(m_port);
  sin.sin_family = AF_INET;
  if (::connect(m_sock, reinterpret_cast<struct sockaddr *>(&sin),
                sizeof(struct sockaddr)) == -1)
    {
      throw CommunicationError("Failed to connect to server");
    }
  toggleTimeout();
  m_readFd = m_sock;
  m_writeFd = m_sock;
  nope::log::Log(Debug) << "Configured client";
}

void InternetSocket::configureHost()
{
  struct sockaddr_in sin;

  m_isHost = true;
  m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
  if (m_sock == -1)
    {
      throw CommunicationError("Failed to create socket");
    }
  int yes = 1;
  ::setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(m_port);
  if (::bind(m_sock, reinterpret_cast<struct sockaddr *>(&sin),
             sizeof(struct sockaddr_in)) == -1)
    {
      throw CommunicationError("Failed to bind socket");
    }
  if (::listen(m_sock, 1) == -1)
    {
      throw CommunicationError("Failed to listen on socket");
    }

  // Wait for connection
  nope::log::Log(Debug) << "Host waiting for client";
  do
    {
      int rc;
      do
	{
	  fd_set readfds;
	  FD_ZERO(&readfds);
	  FD_SET(m_sock, &readfds);
	  rc = ::select(m_sock + 1, &readfds, nullptr, nullptr, nullptr);
	}
      while (rc == -1 && errno == EINTR);
      if (rc == -1)
	{
	  throw CommunicationError("Cannot wait for client");
	}
      struct sockaddr_in clisin;
      socklen_t          sinsize = sizeof(struct sockaddr_in);
      m_cliSock = ::accept(
          m_sock, reinterpret_cast<struct sockaddr *>(&clisin), &sinsize);
    }
  while (m_cliSock == -1);
  m_readFd = m_cliSock;
  m_writeFd = m_cliSock;
  nope::log::Log(Debug) << "Client connected to host !";
  nope::log::Log(Debug) << "Configured host";
}