#ifndef PROCESSLIST_HPP_
#define PROCESSLIST_HPP_

#include <algorithm>
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

  void removeProcess(Process<T> const &p)
  {
    nope::log::Log(Debug) << "Removing process from process list";
    m_proc.erase(std::remove(m_proc.begin(), m_proc.end(), p), m_proc.end());
  }

  size_t getNbProcesses() const
  {
    return (m_proc.size());
  }

  void wait()
  {
    nope::log::Log(Debug) << "Waiting all processes to finish.";
    for (Process<T> &t : m_proc)
      {
	t.wait();
      }
    nope::log::Log(Debug) << "All processes are finished.";
  }

  // Handle loadbalancing
  void loadbalance(Order const &ord)
  {
    static std::uint32_t i = 0;
    bool                 sent = false;

    while (sent == false)
      {
	// Reinitialize index
	if (i >= m_proc.size())
	  {
	    i = 0;
	  }

	// Send to process
	for (typename std::vector<Process<T>>::iterator p = m_proc.begin() + i;
	     p != m_proc.end();)
	  {
	    bool changed = false;
	    if (p->isAvailable())
	      {
		ICommunicable const &com = p->getCommunication();
		Message<Order>       msg;

		msg << ord;
		if (com.write(msg) == true)
		  {
		    nope::log::Log(Debug) << "Sent order";
		    sent = true;
		    break;
		  }
		else
		  {
		    changed = true;
		    p = m_proc.erase(p);
		  }
	      }
	    if (changed == false)
	      {
		++p;
	      }
	  }
	// There was no process available, so add one
	if (sent == false)
	  {
	    nope::log::Log(Debug)
	        << "Couldn't send order, creating a new process";
	    addProcess();
	    i = static_cast<std::uint32_t>(m_proc.size());
	    if (i)
	      --i;
	  }
      }
    ++i;
  }

  std::vector<Process<T>> const &getProcesses() const
  {
    return (m_proc);
  }

  std::vector<Process<T>> &getProcesses()
  {
    return (m_proc);
  }

private:
  std::vector<Process<T>> m_proc;
  size_t                  m_threadPerProcess;
};

#endif // !PROCESSLIST_HPP_
