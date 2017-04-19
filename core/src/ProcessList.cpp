#include "ProcessList.hpp"
#include "Logger.hpp"

ProcessList::ProcessList(size_t nbThread)
    : m_proc(), m_threadPerProcess(nbThread)
{
}

ProcessList::~ProcessList()
{
}

ProcessList &ProcessList::operator++()
{
  addProcess();
  return (*this);
}

bool ProcessList::addProcess()
{
  nope::log::Log(Debug) << "Pushing";
  m_proc.push_back(Process(m_threadPerProcess));
  m_proc.back().run();
  nope::log::Log(Debug) << "Added process to process list";
  return (true);
}

void ProcessList::checkTimeout()
{
  for (Process &t : m_proc)
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

void ProcessList::loadbalance()
{
  if (getNbProcesses() == 0)
    {
      addProcess();
    }
  // TODO
}

size_t ProcessList::getNbProcesses() const
{
  return (m_proc.size());
}
