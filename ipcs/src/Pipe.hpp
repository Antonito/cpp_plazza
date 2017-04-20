#ifndef PIPE_HPP_
#define PIPE_HPP_

#include "FileDescriptorCommunicable.hpp"

class Pipe : public FileDescriptorCommunicable
{
public:
  Pipe();
  virtual ~Pipe(){};

  virtual bool write(IMessage const &) const;
  virtual bool read(IMessage &);
  virtual void configureClient();
  virtual void configureHost();

private:
  static constexpr size_t buffSize = 4096;
  enum
  {
    PIPE_READ = 0,
    PIPE_WRITE,
    NB_PIPES
  };
  int m_pipesIn[NB_PIPES];
  int m_pipesOut[NB_PIPES];
};

#endif // !PIPE_HPP_
