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
}

void FileDescriptorCommunicable::close()
{
  bool const  isSame = (m_readFd == m_writeFd);
  pid_t const pid = getpid();

  if (m_readFd > 0)
    {
      nope::log::Log(Debug) << "Closing Fd " << m_readFd << " {" << pid << "}";
      ::close(m_readFd);
    }
  if (!isSame && m_writeFd > 0)
    {
      nope::log::Log(Debug) << "Closing Fd " << m_writeFd << " {" << pid
                            << "}";
      ::close(m_writeFd);
    }
}

bool FileDescriptorCommunicable::canWrite(bool const blocking) const
{
  int            rc;
  fd_set         fds;
  struct timeval tm;
  pid_t const    pid = getpid();

#if defined(DEBUG_VERBOSE)
  nope::log::Log(Debug) << "Checking if can write {" << pid << "}";
#endif
  do
    {
      FD_ZERO(&fds);
      FD_SET(m_writeFd, &fds);
      tm.tv_sec = (m_timeout) ? 2 : 0;
      tm.tv_usec = 0;
      rc = select(m_writeFd + 1, nullptr, &fds, nullptr,
                  (blocking) ? nullptr : &tm);
    }
  while (rc == -1 && errno == EINTR);
  if (rc <= 0)
    {
      if (rc == -1)
	{
	  nope::log::Log(Error) << "Select failed {" << pid << "}";
	}
      nope::log::Log(Debug) << "Not ready to write {" << pid << "}";
      return (false);
    }
#if defined(DEBUG_VERBOSE)
  nope::log::Log(Debug) << "Ready to write {" << pid << "}";
#endif
  return (true);
}

bool FileDescriptorCommunicable::canRead(bool const blocking) const
{
  int            rc;
  fd_set         fds;
  struct timeval tm;
  pid_t const    pid = getpid();

#if defined(DEBUG_VERBOSE)
  nope::log::Log(Debug) << "Checking if can read {" << pid << "}";
#endif
  do
    {
      FD_ZERO(&fds);
      FD_SET(m_readFd, &fds);
      tm.tv_sec = (m_timeout) ? 2 : 0;
      tm.tv_usec = 0;
      rc = select(m_readFd + 1, &fds, nullptr, nullptr,
                  (blocking) ? nullptr : &tm);
    }
  while (rc == -1 && errno == EINTR);

  if (rc <= 0)
    {
      if (rc == -1)
	{
	  nope::log::Log(Error) << "Select failed {" << pid << "}";
	}
      nope::log::Log(Debug) << "Not ready to read {" << pid << "}";
      return (false);
    }
#if defined(DEBUG_VERBOSE)
  nope::log::Log(Debug) << "Ready to read {" << pid << "}";
#endif
  return (true);
}

int FileDescriptorCommunicable::getWriteHandler() const
{
  return (m_writeFd);
}

int FileDescriptorCommunicable::getReadHandler() const
{
  return (m_readFd);
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