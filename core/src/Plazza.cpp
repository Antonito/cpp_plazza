#include <sstream>
#include <cstring>
#include "Plazza.hpp"
#include "Logger.hpp"
#include "Order.hpp"
#include "OrderList.hpp"

Plazza::Plazza(std::int32_t nbThreads)
    : m_nbThreads(nbThreads),
      m_processes(static_cast<std::size_t>(m_nbThreads))
{
}

Plazza::~Plazza()
{
}

void Plazza::loop()
{
  int stdin = STDIN_FILENO;

  nope::log::Log(Info) << "Starting processing.";
  while (1)
    {
      std::string       input;
      std::stringstream ss;
      OrderList         orderList;
      int               ret;
      fd_set            readfds;

      do
	{
	  int            maxFd;
	  struct timeval tm;

	  maxFd = stdin;

	  // Set 1 seconds timer
	  tm.tv_sec = 1;
	  tm.tv_usec = 0;

	  FD_ZERO(&readfds);

	  // Add stdin, if needed
	  if (stdin != -1)
	    {
	      FD_SET(stdin, &readfds);
	    }

	  // Add process file descriptors
	  for (Process<Comm_t> const &p : m_processes.getProcesses())
	    {
	      int readFd = p.getCommunication().getReadHandler();

	      FD_SET(readFd, &readfds);
	      nope::log::Log(Debug) << "Adding FD " << readFd;
	      if (readFd >= maxFd)
		{
		  maxFd = readFd;
		}
	    }
	  ret = select(maxFd + 1, &readfds, nullptr, nullptr, &tm);
	}
      while (ret == -1 && errno == EINTR);

      if (ret == -1)
	{
	  nope::log::Log(Error) << "select() failed : "
	                        << std::strerror(errno);
	  break;
	}
      else if (ret == 0)
	{
	  // If there are no more processes, exit
	  nope::log::Log(Debug) << "Main process timed out ["
	                        << m_processes.getNbProcesses()
	                        << " proc left]";
	  if (m_processes.getNbProcesses() == 0)
	    {
	      nope::log::Log(Debug) << "Not any processes left...";
	      break;
	    }
	}
      else
	{
	  if (stdin != -1 && FD_ISSET(stdin, &readfds))
	    {
	      if (!std::getline(std::cin, input, '\n'))
		{
		  stdin = -1;
		}
	      else if (std::cin)
		{
		  // Parse input
		  ss << input;
		  while (OrderList::parse(orderList, ss))
		    {
		      // Exec
		      for (size_t i = 0; i < orderList.size(); ++i)
			{
			  m_processes.loadbalance(orderList.getOrder(i));
			}
		    }
		}
	    }

	  // Read data coming from processes
	  for (std::vector<Process<Comm_t>>::iterator p =
	           m_processes.getProcesses().begin();
	       p != m_processes.getProcesses().end();)
	    {
	      bool changed = false;
	      int  readFd = p->getCommunication().getReadHandler();

	      if (FD_ISSET(readFd, &readfds))
		{
		  if (p->getCommunication().read(p->getResponse()) == false)
		    {
		      // Did the process die ?
		      changed = true;
		      m_processes.removeProcess(*p);
		    }
		}

	      // If we didn't remove a process, go to the next one
	      if (changed == false)
		{
		  ++p;
		}
	    }
	}
    }
  nope::log::Log(Info) << "Processing over.";
  m_processes.wait();
  nope::log::Log(Debug) << "Leaving log";
}