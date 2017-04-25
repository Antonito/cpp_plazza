#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <cstddef>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>
#include <cassert>
#include <csignal>
#include <unistd.h>
#include "ThreadPool.hpp"
#include "ICommunicable.hpp"
#include "Logger.hpp"
#include "Message.hpp"
#include "Worker.hpp"
#include "Response.hpp"

template <typename T>
class Process
{
  static_assert(std::is_base_of<ICommunicable, T>(),
                "Type is not an ICommunicable");

public:
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);

  explicit Process(size_t nbThread)
      : m_pid(0), m_ppid(0), m_pool(), m_running(false), m_nbThread(nbThread),
        m_lastAction(std::chrono::system_clock::now()), m_mes(), m_sem(0),
        m_resp(), m_isHost()
  {
  }

  ~Process(){};

  Process(Process const &) = delete;
  Process &operator=(Process const &) = delete;

  Process(Process &&other)
      : m_pid(other.m_pid), m_ppid(other.m_ppid), m_pool(),
        m_running(other.m_running), m_nbThread(other.m_nbThread),
        m_lastAction(other.m_lastAction), m_mes(other.m_mes),
        m_sem(std::move(other.m_sem)), m_resp(other.m_resp),
        m_isHost(other.m_isHost)
  {
    m_pool = std::move(other.m_pool);
  }

  Process &operator=(Process &&other)
  {
    if (this != &other)
      {
	m_pid = other.m_pid;
	m_ppid = other.m_ppid;
	m_pool = std::move(other.m_pool);
	m_running = other.m_running;
	m_nbThread = other.m_nbThread;
	m_lastAction = other.m_lastAction;
	m_mes = other.m_mes;
	m_sem = std::move(other.m_sem);
	m_resp = other.m_resp;
	m_isHost = other.m_isHost;
      }
    return (*this);
  }

  bool operator==(Process const &p2) const
  {
    return (m_pid == p2.m_pid && m_lastAction == p2.m_lastAction);
  }

  bool run()
  {
    assert(m_running == false);
    m_ppid = getpid();
    m_pid = fork();
    if (m_pid == -1)
      {
	return (false);
      }
    else
      {
	m_running = true;
	if (m_pid == 0)
	  {
	    // Child !
	    m_isHost = false;
	    m_mes.configureClient();
	    m_pid = getpid();
	    for (size_t i = 0; i < m_nbThread; ++i)
	      {
		m_pool.addThread();
	      }

	    try
	      {
		_loop();
	      }
	    catch (std::exception const &e)
	      {
		std::cerr << e.what() << std::endl;
	      }
	    nope::log::Log(Info) << "Stopping process " << m_pid;

	    // Prevents memory leaks
	    m_pool.~ThreadPool();
	    nope::log::Log(Debug) << "Exit child process.";
	    exit(0);
	  }
      }
    m_isHost = true;
    m_mes.configureHost();
    nope::log::Log(Debug) << "Process started.";
    return (true);
  }

  bool isRunning() const
  {
    return (m_running);
  }

  bool wait()
  {
    assert(m_running == true);
    nope::log::Log(Debug) << "Waiting for child process.";
    if (::waitpid(m_pid, NULL, 0) == -1)
      {
	return (false);
      }
    nope::log::Log(Debug) << "Child process terminated.";
    m_running = false;
    return (true);
  }

  void kill()
  {
    if (m_running == true)
      {
	nope::log::Log(Debug) << "Killing child process.";
	::kill(m_pid, SIGTERM);
	wait();
      }
  }

  std::chrono::milliseconds getTimeSinceLastAction() const
  {
    return (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - m_lastAction));
  }

  bool hasTimedOut() const
  {
    if (m_sem.getValue() != 0)
      {
	// There is a job running
	return (false);
      }
    return (getTimeSinceLastAction() > Process<T>::timeout);
  }

  bool isAvailable() const
  {
    if (m_isHost == false)
      {
	if (m_sem.getValue() < 2 * m_nbThread)
	  {
	    return (true);
	  }
      }
    else
      {
	// TODO: send message
	return (true);
      }
    return (false);
  }

  ICommunicable const &getCommunication() const
  {
    return (m_mes);
  }

  ICommunicable &getCommunication()
  {
    return (m_mes);
  }

  Message<Response> &getResponse()
  {
    return (m_resp);
  }

private:
  void updateLastAction()
  {
    m_lastAction = std::chrono::system_clock::now();
  }

  void treatOrder(Order const &order)
  {
    m_sem.post(); // Signal the main thread there is a job running
    {
      Worker work;

      nope::log::Log(Info) << "Parsing file " << order.getFile();
      work.exec(order);
    }
    m_sem.wait(); // Signal the main thread the job is over
  }

  void _loop()
  {
    while (m_running)
      {
	Message<Order> msgOrder;
	Order          order;
	bool           newData = false;

	nope::log::Log(Debug) << "Process " << m_pid << " waiting for order ["
	                      << m_sem.getValue() << " tasks]";
	updateLastAction();
	newData = m_mes.read(msgOrder);
	if (newData)
	  {
	    if (isAvailable())
	      {
		// TODO : send positive response
		nope::log::Log(Debug) << "Process " << m_pid
		                      << " got new order";
		msgOrder >> order;
		m_pool.execute(&Process<T>::treatOrder, this, order);
	      }
	    else
	      {
		// TODO: Send negative response
	      }
	  }

	if (hasTimedOut())
	  {
	    nope::log::Log(Debug) << "Process " << m_pid << " timed out.";
	    m_running = false;
	    break;
	  }
      }
  }
  pid_t                                 m_pid;
  pid_t                                 m_ppid;
  ThreadPool                            m_pool;
  bool                                  m_running;
  size_t                                m_nbThread;
  std::chrono::system_clock::time_point m_lastAction;
  T                                     m_mes;
  Semaphore                             m_sem;
  Message<Response>                     m_resp;
  bool                                  m_isHost;
};

#endif // !PROCESS_HPP_
