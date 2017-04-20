#ifndef PROCESSLIST_HPP_
#define PROCESSLIST_HPP_

#include "Order.hpp"
#include "Process.hpp"
#include "Logger.hpp"

template <typename T>
constexpr std::chrono::seconds Process<T>::timeout;

template <typename T>
class ProcessList
{
  static_assert(std::is_base_of<ICommunicable, T>(),
                "Type is not an ICommunicable");

public:
  ProcessList(size_t nbThread) : m_proc(), m_threadPerProcess(nbThread)
  {
  }

  ~ProcessList(){};

  ProcessList(ProcessList const &) = delete;
  ProcessList &operator=(ProcessList const &) = delete;

  ProcessList &operator++()
  {
    addProcess();
    return (*this);
  }

  bool addProcess()
  {
    nope::log::Log(Debug) << "Pushing";
    m_proc.push_back(Process<T>(m_threadPerProcess));
    m_proc.back().run();
    nope::log::Log(Debug) << "Added process to process list";
    return (true);
  }

  size_t getNbProcesses() const
  {
    return (m_proc.size());
  }

  // Handle timeout
  void checkTimeout()
  {
    for (Process<T> &t : m_proc)
      {
#if 0
      if (t.getTimeSinceLastAction() >= timeout)
	{
	  // TODO: remove process
	  t.kill();
	}
#endif
      }
  }

  // Handle loadbalancing
  void loadbalance(Order const &ord)
  {
    if (getNbProcesses() == 0)
      {
	addProcess();
      }
    // TODO
  }

private:
  std::vector<Process<T>> m_proc;
  size_t                  m_threadPerProcess;
};

#endif // !PROCESSLIST_HPP_
