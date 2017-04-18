#include <sys/time.h>
#include <sys/select.h>
#include "FileDescriptorCommunicable.hpp"

bool FileDescriptorCommunicable::canWrite() const
{
  int            rc;
  fd_set         fds;
  struct timeval tm;

  FD_ZERO(&fds);
  FD_SET(writeFd, &fds);
  tm.tv_sec = 0;
  tm.tv_usec = 0;
  rc = select(writeFd + 1, nullptr, &fds, nullptr, &tm);
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

  FD_ZERO(&fds);
  FD_SET(readFd, &fds);
  tm.tv_sec = 0;
  tm.tv_usec = 0;
  rc = select(readFd + 1, &fds, nullptr, nullptr, &tm);
  if (rc <= 0)
    {
      return (false);
    }
  return (true);
}