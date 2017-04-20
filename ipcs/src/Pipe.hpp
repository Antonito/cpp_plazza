#ifndef PIPE_HPP_
#define PIPE_HPP_

#include "FileDescriptorCommunicable.hpp"

class Pipe : public FileDescriptorCommunicable
{
public:
  Pipe();
  virtual ~Pipe(){};

  virtual bool write(IMessage const &);
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

#endif // !PIPE_HPP_
