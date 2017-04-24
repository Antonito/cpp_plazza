#include <cassert>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Pipe.hpp"
#include "Logger.hpp"
#include "CommunicationError.hpp"

constexpr size_t Pipe::buffSize;

Pipe::Pipe() : FileDescriptorCommunicable(), m_pipesIn(), m_pipesOut()
{
  if (pipe(m_pipesIn) == -1 || pipe(m_pipesOut) == -1)
    {
      throw CommunicationError(
          "Cannot open pipe"); // TODO: adjust error message?
    }
}

bool Pipe::write(IMessage const &m) const
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

bool Pipe::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      uint32_t size;

      if (::read(m_readFd, &size, sizeof(uint32_t)) > 0)
	{
	  size = ntohl(size);
	  assert(size < Pipe::buffSize);

	  std::unique_ptr<uint8_t[]> buff =
	      std::make_unique<uint8_t[]>(Pipe::buffSize);
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

void Pipe::configureClient()
{
  close(m_pipesIn[PIPE_WRITE]);
  m_pipesIn[PIPE_WRITE] = -1;
  m_readFd = m_pipesIn[PIPE_READ];

  close(m_pipesOut[PIPE_READ]);
  m_pipesOut[PIPE_READ] = -1;
  m_writeFd = m_pipesOut[PIPE_WRITE];
  toggleTimeout();
  nope::log::Log(Debug) << "Configured client";
}

void Pipe::configureHost()
{
  close(m_pipesIn[PIPE_READ]);
  m_pipesIn[PIPE_READ] = -1;
  m_writeFd = m_pipesIn[PIPE_WRITE];

  close(m_pipesOut[PIPE_WRITE]);
  m_pipesOut[PIPE_WRITE] = -1;
  m_readFd = m_pipesOut[PIPE_READ];
  nope::log::Log(Debug) << "Configured host";
}