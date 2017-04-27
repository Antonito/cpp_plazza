#ifndef NAMED_PIPE_HPP_
#define NAMED_PIPE_HPP_

#include "FileDescriptorCommunicable.hpp"

class NamedPipe : public FileDescriptorCommunicable
{
public:
  NamedPipe();
  virtual ~NamedPipe();

  virtual bool write(IMessage const &) const;
  virtual bool read(IMessage &);
  virtual void configureClient();
  virtual void configureHost();

private:
  static constexpr size_t  buffSize = 4096;
  static std::int32_t      ndx;
  static std::string const fifoIn;
  static std::string const fifoOut;

  enum
  {
    PIPE_READ = 0,
    PIPE_WRITE,
    NB_PIPES
  };
  int         m_pipesIn[NB_PIPES];
  int         m_pipesOut[NB_PIPES];
  bool        m_isHost;
  std::string m_fifoIn;
  std::string m_fifoOut;
};

#endif // !NAMED_PIPE_HPP_
