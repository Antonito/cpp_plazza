#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include "FileDescriptorCommunicable.hpp"
#include "Logger.hpp"

FileDescriptorCommunicable::FileDescriptorCommunicable()
    : m_readFd(-1), m_writeFd(-1), m_timeout(false)
{
}

FileDescriptorCommunicable::~FileDescriptorCommunicable()
{
  if (m_readFd > 0)
    {
      close(m_readFd);
    }
  if (m_writeFd > 0)
    {
      close(m_writeFd);
    }
}

bool FileDescriptorCommunicable::canWrite() const
{
  int            rc;
  fd_set         fds;
  struct timeval tm;

  do
    {
      FD_ZERO(&fds);
      FD_SET(m_writeFd, &fds);
      tm.tv_sec = (m_timeout) ? 5 : 0;
      tm.tv_usec = 0;
      rc = select(m_writeFd + 1, nullptr, &fds, nullptr, &tm);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      if (rc == -1)
	{
	  nope::log::Log(Error) << "Select failed";
	}
      nope::log::Log(Debug) << "Not ready to write";
      return (false);
    }
  nope::log::Log(Debug) << "Ready to write";
  return (true);
}

bool FileDescriptorCommunicable::canRead() const
{
  int            rc;
  fd_set         fds;
  struct timeval tm;

  do
    {
      FD_ZERO(&fds);
      FD_SET(m_readFd, &fds);
      tm.tv_sec = (m_timeout) ? 5 : 0;
      tm.tv_usec = 0;
      rc = select(m_readFd + 1, &fds, nullptr, nullptr, &tm);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      if (rc == -1)
	{
	  nope::log::Log(Error) << "Select failed";
	}
      nope::log::Log(Debug) << "Not ready to read";
      return (false);
    }
  nope::log::Log(Debug) << "Ready to read";
  return (true);
}

void FileDescriptorCommunicable::toggleTimeout()
{
  m_timeout = !m_timeout;
}

FileDescriptorCommunicable &FileDescriptorCommunicable::
    operator<<(IMessage const &m)
{
  write(m);
  return (*this);
}

FileDescriptorCommunicable &FileDescriptorCommunicable::operator>>(IMessage &m)
{
  read(m);
  return (*this);
}