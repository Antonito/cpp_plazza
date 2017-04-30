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
        m_lastAction(std::chrono::system_clock::now()), m_mes(), m_resp(),
        m_isHost()
  {
  }

  ~Process(){};

  Process(Process const &) = delete;
  Process &operator=(Process const &) = delete;

  Process(Process &&other)
      : m_pid(other.m_pid), m_ppid(other.m_ppid), m_pool(),
        m_running(other.m_running), m_nbThread(other.m_nbThread),
        m_lastAction(other.m_lastAction), m_mes(other.m_mes),
        m_resp(other.m_resp), m_isHost(other.m_isHost)
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
	    m_pid = getpid();
	    nope::log::Log(Info) << "Starting process " << m_pid;
	    m_isHost = false;
	    m_mes.configureClient();
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
		nope::log::Log(Error)
		    << "{" << m_pid << "} Error: " << e.what();
	      }
	    nope::log::Log(Info) << "Stopping process " << m_pid;

	    m_mes.close();
	    // Prevents memory leaks
	    m_pool.~ThreadPool();
	    nope::log::Log(Debug) << "Exit child process " << m_pid;
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
    nope::log::Log(Debug) << "Waiting for child process.";
    if (::waitpid(m_pid, NULL, 0) == -1)
      {
	return (false);
      }
    nope::log::Log(Debug) << "Child process terminated.";
    m_mes.close(); // Close connection
    m_running = false;
    return (true);
  }

  void kill()
  {
    if (m_running == true)
      {
	nope::log::Log(Debug) << "Killing child process.";
	::kill(m_pid, SIGTERM);
	while (!wait())
	  ;
      }
  }

  std::chrono::milliseconds getTimeSinceLastAction() const
  {
    return (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - m_lastAction));
  }

  bool hasTimedOut() const
  {
    if (m_pool.getNumberTasks() != 0)
      {
	// There is a job running
	return (false);
      }
    return (getTimeSinceLastAction() > Process<T>::timeout);
  }

  bool isAvailable()
  {
    Message<Response> respPck;
    bool              ret = false;

    if (m_isHost == false)
      {
	// Client part (process)
	if (m_pool.getNumberTasks() < 2 * m_nbThread)
	  {
	    ret = true;
	  }
	if (m_mes.canRead())
	  {
	    Response rep;

	    if (m_mes.read(respPck))
	      {
		respPck >> rep;
		if (rep.isAvailable() && m_mes.canWrite(true))
		  {

		    std::vector<bool> const &status = m_pool.getThreadStatus();
		    uint32_t                 busy = 0;

		    for (bool b : status)
		      {
			if (b)
			  {
			    ++busy;
			  }
		      }

		    uint32_t waiting = m_pool.getNumberTasks();

		    Response resp(ret, busy, waiting);
		    // TODO: Fill threadpool data here
		    respPck << resp;
		    m_mes << respPck;
#if defined(DEBUG_VERBOSE)
		    nope::log::Log(Debug)
		        << "Process " << m_pid << " sent response";
#endif
		  }
	      }
	  }
      }
    else
      {
	// Host part (main process)
	Response resp(true, 42, 42);

	nope::log::Log(Debug)
	    << "Waiting for response from process [Available ?]";
	if (m_mes.canWrite(true))
	  {
	    respPck << resp;
	    m_mes << respPck;
	    if (m_mes.canRead(true))
	      {
		m_mes >> respPck;
		respPck >> resp;
		m_resp = respPck;
		ret = resp.isAvailable();
	      }
	    nope::log::Log(Debug) << "Got response ! Available: " << ret;
	  }
      }
    return (ret);
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
    Worker work;

    nope::log::Log(Info) << "Parsing file " << order.getFile();
    work.exec(order);
  }

  void _loop()
  {
    while (m_running)
      {
	Message<Order> msgOrder;
	Order          order;
	bool           newData = false;

	nope::log::Log(Debug) << "Process " << m_pid << " waiting for order ["
	                      << m_pool.getNumberTasks() << " tasks]";
	updateLastAction();
	if (isAvailable())
	  {
	    // We're waiting for an order
	    do
	      {
		if (m_mes.canRead())
		  {
		    newData = m_mes.read(msgOrder);
		    if (newData)
		      {
			nope::log::Log(Debug)
			    << "Process " << m_pid << " got new order";
			msgOrder >> order;
			m_pool.execute(&Process<T>::treatOrder, this, order);
		      }
		  }
		if (hasTimedOut())
		  {
		    break;
		  }
	      }
	    while (!newData);
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
  Message<Response>                     m_resp;
  bool                                  m_isHost;
};

#endif // !PROCESS_HPP_
