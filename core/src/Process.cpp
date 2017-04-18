#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <iostream>
#include "Process.hpp"
#include "Logger.hpp"

Process::Process(size_t nbThread)
    : m_pid(0), m_ppid(0), m_pool(), m_running(false), m_nbThread(nbThread),
      m_lastAction(std::chrono::system_clock::now())
{
}

Process::~Process()
{
}

Process::Process(Process &&other)
    : m_pid(other.m_pid), m_ppid(other.m_ppid), m_running(other.m_running),
      m_nbThread(other.m_nbThread), m_lastAction(other.m_lastAction)
{
}

Process &Process::operator=(Process &&other)
{
  if (this != &other)
    {
      m_pid = other.m_pid;
      m_ppid = other.m_ppid;
      //      m_pool = std::move(other.m_pool);
      m_running = other.m_running;
      m_nbThread = other.m_nbThread;
      m_lastAction = other.m_lastAction;
    }
  return (*this);
}

bool Process::run()
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
	  for (size_t i = 0; i < m_nbThread; ++i)
	    {
	      m_pool.addThread();
	    }

	  try
	    {
	      _loop();
	    }
	  catch (...)
	    {
	      std::cerr << "An error occured." << std::endl;
	    }

	  // Prevents memory leaks
	  m_pool.~ThreadPool();
	  exit(0);
	}
    }
#if defined(DEBUG)
  Nope::Log::Debug << "Process started.";
#endif
  return (true);
}

bool Process::isRunning() const
{
  return (m_running);
}

bool Process::wait()
{
  assert(m_running == true);
#if defined(DEBUG)
  Nope::Log::Debug << "Waiting for child process.";
#endif
  if (::waitpid(m_pid, NULL, 0) == -1)
    {
      return (false);
    }
#if defined(DEBUG)
  Nope::Log::Debug << "Child process terminated.";
#endif
  m_running = false;
  return (true);
}

void Process::_loop()
{
  // TODO: Wait for orders and execute
}

std::chrono::milliseconds Process::getTimeSinceLastAction() const
{
  return (std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now() - m_lastAction));
}

void Process::kill()
{
  assert(m_running == true);
#if defined(DEBUG)
  Nope::Log::Debug << "Killing child process.";
#endif
  ::kill(m_pid, SIGTERM);
  wait();
}
