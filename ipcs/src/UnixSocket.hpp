#ifndef UNIX_SOCKET_HPP_
#define UNIX_SOCKET_HPP_

#include "FileDescriptorCommunicable.hpp"

class UnixSocket : public FileDescriptorCommunicable
{
public:
  UnixSocket();
  virtual ~UnixSocket(){};

  virtual bool write(IMessage const &) const;
  virtual bool read(IMessage &);
  virtual void configureClient();
  virtual void configureHost();

private:
  static constexpr size_t buffSize = 4096;
  enum
  {
    SOCK_HOST = 0,
    SOCK_CLIENT,
    NB_SOCKS
  };
  int m_socks[NB_SOCKS];
};

#endif // !UNIX_SOCKET_HPP_
