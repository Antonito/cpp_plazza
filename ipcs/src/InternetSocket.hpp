#ifndef INTERNET_SOCKET_HPP_
#define INTERNET_SOCKET_HPP_

#include "FileDescriptorCommunicable.hpp"

class InternetSocket : public FileDescriptorCommunicable
{
public:
  virtual ~InternetSocket(){};

  virtual bool write(IMessage const &);
  virtual bool read(IMessage &);
};

#endif // !INTERNET_SOCKET_HPP_
