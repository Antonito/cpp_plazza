#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include "FileDescriptorCommunicable.hpp"
// TODO: overload << and >>
FileDescriptorCommunicable::FileDescriptorCommunicable()
    : m_readFd(-1), m_writeFd(-1)
{
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
      tm.tv_sec = 0;
      tm.tv_usec = 0;
      rc = select(m_writeFd + 1, nullptr, &fds, nullptr, &tm);
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
  int            rc;
  fd_set         fds;
  struct timeval tm;

  do
    {
      FD_ZERO(&fds);
      FD_SET(m_readFd, &fds);
      tm.tv_sec = 0;
      tm.tv_usec = 0;
      rc = select(m_readFd + 1, &fds, nullptr, nullptr, &tm);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      return (false);
    }
  return (true);
}