#include <cassert>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "Pipe.hpp"

constexpr size_t Pipe::buffSize;

Pipe::Pipe() : FileDescriptorCommunicable(), m_pipesIn(), m_pipesOut()
{
  if (pipe(m_pipesIn) == -1 || pipe(m_pipesOut) == -1)
    {
      throw std::exception(); // TODO
    }
}

bool Pipe::write(IMessage const &m) const
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
  close(m_pipesIn[PIPE_WRITE]);
  m_pipesIn[PIPE_WRITE] = -1;
  m_readFd = m_pipesIn[PIPE_READ];

  close(m_pipesOut[PIPE_READ]);
  m_pipesOut[PIPE_READ] = -1;
  m_writeFd = m_pipesOut[PIPE_WRITE];
}

void Pipe::configureHost()
{
  close(m_pipesIn[PIPE_READ]);
  m_pipesIn[PIPE_READ] = -1;
  m_writeFd = m_pipesIn[PIPE_WRITE];

  close(m_pipesOut[PIPE_WRITE]);
  m_pipesOut[PIPE_WRITE] = -1;
  m_readFd = m_pipesOut[PIPE_READ];
}