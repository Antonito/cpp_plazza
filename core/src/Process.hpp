#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <cstddef>
#include <sys/types.h>
#include "ThreadPool.hpp"

class Process
{
public:
  explicit Process(size_t nbThread);
  ~Process();
  Process(Process const &) = delete;
  Process &operator=(Process const &) = delete;

  bool run();
  bool isRunning() const;
  bool wait();

private:
  void       _loop();
  pid_t      m_pid;
  pid_t      m_ppid;
  ThreadPool m_pool;
  bool       m_running;
  size_t     m_nbThread;
};

#endif // !PROCESS_HPP_