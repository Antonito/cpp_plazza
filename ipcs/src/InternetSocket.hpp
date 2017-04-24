#ifndef INTERNET_SOCKET_HPP_
#define INTERNET_SOCKET_HPP_

#include <cstdint>
#include "FileDescriptorCommunicable.hpp"

class InternetSocket : public FileDescriptorCommunicable
{
public:
  InternetSocket();
  virtual ~InternetSocket();

  virtual bool write(IMessage const &) const;
  virtual bool read(IMessage &);
  virtual void configureClient();
  virtual void configureHost();

private:
  static constexpr size_t buffSize = 4096;
  int                     m_sock;
  int                     m_cliSock;
  std::uint16_t           m_port;
  bool                    m_isHost;
  static std::uint16_t    portOff;
};

#endif // !INTERNET_SOCKET_HPP_
