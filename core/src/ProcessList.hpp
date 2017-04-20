#ifndef PROCESSLIST_HPP_
#define PROCESSLIST_HPP_

#include "Order.hpp"
#include "Process.hpp"

class ProcessList
{
public:
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
  void loadbalance(Order &ord);

private:
  std::vector<Process> m_proc;
  size_t               m_threadPerProcess;
};

#endif // !PROCESSLIST_HPP_
