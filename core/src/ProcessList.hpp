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

  void wait()
  {
    nope::log::Log(Debug) << "Waiting all processes to finish.";
    for (Process<T> &t : m_proc)
      {
	t.wait();
      }
    nope::log::Log(Debug) << "All processes are finished.";
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
    // TODO: Loadbalance
    bool sent = false;

    while (sent == false)
      {
	if (getNbProcesses() == 0)
	  {
	    addProcess();
	  }

	// Send to process
	for (typename std::vector<Process<T>>::iterator p = m_proc.begin();
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
      }
  }

private:
  std::vector<Process<T>> m_proc;
  size_t                  m_threadPerProcess;
};

#endif // !PROCESSLIST_HPP_
