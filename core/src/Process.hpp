#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <cstddef>
#include <sys/types.h>
#include <chrono>
#include "ThreadPool.hpp"

class Process
{
public:
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);

  explicit Process(size_t nbThread);
  ~Process();
  Process(Process const &) = delete;
  Process &operator=(Process const &) = delete;

  Process(Process &&);
  Process &operator=(Process &&);

  bool run();
  bool isRunning() const;
  bool wait();

  void kill();

  std::chrono::milliseconds getTimeSinceLastAction() const;
  bool                      hasTimedOut() const;

private:
  void updateLastAction();

  void                                  _loop();
  pid_t                                 m_pid;
  pid_t                                 m_ppid;
  ThreadPool                            m_pool;
  bool                                  m_running;
  size_t                                m_nbThread;
  std::chrono::system_clock::time_point m_lastAction;
};

#endif // !PROCESS_HPP_