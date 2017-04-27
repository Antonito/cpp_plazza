#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "NamedPipe.hpp"
#include "Logger.hpp"
#include "CommunicationError.hpp"

constexpr size_t  NamedPipe::buffSize;
std::string const NamedPipe::fifoIn = "/tmp/fifo_in";
std::string const NamedPipe::fifoOut = "/tmp/fifo_out";
std::int32_t      NamedPipe::ndx = 0;

NamedPipe::NamedPipe()
    : FileDescriptorCommunicable(), m_pipesIn(), m_pipesOut(), m_isHost(false),
      m_fifoIn(fifoIn + std::to_string(ndx)),
      m_fifoOut(fifoOut + std::to_string(ndx))
{
  if (mkfifo(m_fifoIn.c_str(), 0666) == -1 ||
      mkfifo(m_fifoOut.c_str(), 0666) == -1)
    {
      throw CommunicationError("Cannot open pipe");
    }
  ++ndx;
}

NamedPipe::~NamedPipe()
{
  if (m_isHost)
    {
      unlink(m_fifoIn.c_str());
      unlink(m_fifoOut.c_str());
      --ndx;
    }
}

bool NamedPipe::write(IMessage const &m) const
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

bool NamedPipe::read(IMessage &m)
{
  assert(m_readFd != -1);
  if (canRead())
    {
      uint32_t size;

      if (::read(m_readFd, &size, sizeof(uint32_t)) > 0)
	{
	  size = ntohl(size);
	  assert(size < NamedPipe::buffSize);

	  std::unique_ptr<uint8_t[]> buff =
	      std::make_unique<uint8_t[]>(NamedPipe::buffSize);
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

void NamedPipe::configureClient()
{
  m_readFd = open(m_fifoIn.c_str(), O_RDONLY);
  m_writeFd = open(m_fifoOut.c_str(), O_WRONLY);
  if (m_readFd == -1 || m_writeFd == -1)
    {
      throw CommunicationError("Cannot open named pipe");
    }
  toggleTimeout();
  nope::log::Log(Debug) << "Configured client";
}

void NamedPipe::configureHost()
{
  m_writeFd = open(m_fifoIn.c_str(), O_WRONLY);
  m_readFd = open(m_fifoOut.c_str(), O_RDONLY);
  if (m_readFd == -1 || m_writeFd == -1)
    {
      throw CommunicationError("Cannot open named pipe");
    }
  nope::log::Log(Debug) << "Configured host";
  m_isHost = true;
}