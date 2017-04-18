#ifndef PROCESSLIST_HPP_
#define PROCESSLIST_HPP_

#include <chrono>
#include "Process.hpp"

class ProcessList
{
public:
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);

  ProcessList(size_t nbThread);
  ~ProcessList();

  ProcessList(ProcessList const &) = delete;
  ProcessList &operator=(ProcessList const &) = delete;

  ProcessList &operator++();

  bool   addProcess();
  size_t getNbProcesses() const;

  // Handle timeout
  void checkTimeout();

  // Handle loadbalancing
  void loadbalance(); // TODO : Command

private:
  std::vector<Process> m_proc;
  size_t               m_threadPerProcess;
};

#endif // !PROCESSLIST_HPP_
