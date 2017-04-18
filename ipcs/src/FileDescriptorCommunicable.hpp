#ifndef FILE_DESCRIPTOR_COMMUNICABLE
#define FILE_DESCRIPTOR_COMMUNICABLE

#include "ICommunicable.hpp"

class FileDescriptorCommunicable : public ICommunicable
{
public:
  virtual ~FileDescriptorCommunicable(){};

  virtual bool write(IMessage const &) = 0;
  virtual bool read(IMessage &) = 0;

  virtual bool canWrite() const;
  virtual bool canRead() const;

protected:
  int readFd;
  int writeFd;
};

#endif // !FILE_DESCRIPTOR_COMMUNICABLE
