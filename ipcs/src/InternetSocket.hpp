#ifndef INTERNET_SOCKET_HPP_
#define INTERNET_SOCKET_HPP_

#include "FileDescriptorCommunicable.hpp"

class InternetSocket : public FileDescriptorCommunicable
{
public:
  InternetSocket();
  virtual ~InternetSocket(){};

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

#endif // !INTERNET_SOCKET_HPP_
