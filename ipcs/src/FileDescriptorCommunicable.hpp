#ifndef FILE_DESCRIPTOR_COMMUNICABLE
#define FILE_DESCRIPTOR_COMMUNICABLE

#include "ICommunicable.hpp"

class FileDescriptorCommunicable : public ICommunicable
{
public:
  FileDescriptorCommunicable();
  virtual ~FileDescriptorCommunicable();

  virtual bool write(IMessage const &) const = 0;
  virtual bool read(IMessage &) = 0;
  virtual void configureClient() = 0;
  virtual void configureHost() = 0;

  virtual bool canWrite(bool const blocking = false) const;
  virtual bool canRead(bool const blocking = false) const;

  virtual int getWriteHandler() const;
  virtual int getReadHandler() const;

  void toggleTimeout();

  virtual void close();

  FileDescriptorCommunicable &operator<<(IMessage const &);
  FileDescriptorCommunicable &operator>>(IMessage &);

protected:
  int  m_readFd;
  int  m_writeFd;
  bool m_timeout;
};

#endif // !FILE_DESCRIPTOR_COMMUNICABLE
