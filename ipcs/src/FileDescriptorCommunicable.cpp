#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include "FileDescriptorCommunicable.hpp"

FileDescriptorCommunicable::FileDescriptorCommunicable()
    : m_readFd(-1), m_writeFd(-1), m_timeout(true)
{
}

bool FileDescriptorCommunicable::canWrite() const
{
  int             rc;
  fd_set          fds;
  struct timeval  tm;
  struct timeval *t = nullptr;

  do
    {
      FD_ZERO(&fds);
      FD_SET(m_writeFd, &fds);
      tm.tv_sec = 0;
      tm.tv_usec = 0;
      if (m_timeout)
	{
	  t = &tm;
	}
      rc = select(m_writeFd + 1, nullptr, &fds, nullptr, t);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      return (false);
    }
  return (true);
}

bool FileDescriptorCommunicable::canRead() const
{
  int             rc;
  fd_set          fds;
  struct timeval  tm;
  struct timeval *t = nullptr;

  do
    {
      FD_ZERO(&fds);
      FD_SET(m_readFd, &fds);
      tm.tv_sec = 0;
      tm.tv_usec = 0;
      if (m_timeout)
	{
	  t = &tm;
	}
      rc = select(m_readFd + 1, &fds, nullptr, nullptr, t);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      return (false);
    }
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