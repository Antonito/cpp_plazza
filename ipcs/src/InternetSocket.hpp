#ifndef INTERNET_SOCKET_HPP_
#define INTERNET_SOCKET_HPP_

#include "ICommunicable.hpp"

class NetworkSocket : public ICommunicable
{
public:
  virtual ~NetworkSocket(){};

  virtual bool write(IMessage const &);
  virtual bool read(IMessage const &);

private:
  int m_sock;
};

#endif // !INTERNET_SOCKET_HPP_
