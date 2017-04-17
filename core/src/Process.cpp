#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include "Process.hpp"

Process::Process(size_t nbThread)
    : m_pid(0), m_ppid(0), m_pool(), m_running(false), m_nbThread(nbThread)
{
}

Process::~Process()
{
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
  return (true);
}

bool Process::isRunning() const
{
  return (m_running);
}

bool Process::wait()
{
  assert(m_running == true);
  if (::waitpid(m_pid, NULL, 0) == -1)
    {
      return (false);
    }
  m_running = false;
  return (true);
}

void Process::_loop()
{
  // TODO: Wait for orders and execute
}