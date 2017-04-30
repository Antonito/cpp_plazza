#include <sstream>
#include <cstring>
#include "Plazza.hpp"
#include "Logger.hpp"
#include "Order.hpp"

Plazza::Plazza(std::int32_t nbThreads)
    : m_nbThreads(nbThreads),
      m_processes(static_cast<std::size_t>(m_nbThreads)), m_finished(false),
      m_order(), m_orderList(), m_readfds()
{
}

Plazza::~Plazza()
{
}

void Plazza::update()
{
  if (!m_finished)
    {
      m_finished = this->loop();
    }
  if (m_finished)
    {
      nope::log::Log(Info) << "Processing over.";
      m_processes.wait();
      nope::log::Log(Debug) << "Leaving log";
    }
}

bool Plazza::loop()
{
  int stdin = STDIN_FILENO;

  std::string       input;
  std::stringstream ss;
  int               ret;

  do
    {
      int            maxFd = 0;
      struct timeval tm;

      if (stdin > maxFd)
	{
	  maxFd = stdin;
	}

      // Set 1 seconds timer
      tm.tv_sec = 1;
      tm.tv_usec = 0;

      FD_ZERO(&m_readfds);

      // Add stdin, if needed
      if (stdin != -1)
	{
	  FD_SET(stdin, &m_readfds);
	}

      // Add process file descriptors
      for (Process<Comm_t> const &p : m_processes.getProcesses())
	{
	  int readFd = p.getCommunication().getReadHandler();

	  FD_SET(readFd, &m_readfds);
	  nope::log::Log(Debug) << "Adding FD " << readFd;
	  if (readFd > maxFd)
	    {
	      maxFd = readFd;
	    }
	}
      ret = select(maxFd + 1, &m_readfds, nullptr, nullptr, &tm);
    }
  while (ret == -1 && errno == EINTR);

  if (ret == -1)
    {
      nope::log::Log(Error) << "select() failed : " << std::strerror(errno);
      return (true);
    }
  else if (ret == 0)
    {
      // If there are no more processes, exit
      nope::log::Log(Debug) << "Main process timed out ["
                           << m_processes.getNbProcesses() << " proc left]";
      if (m_processes.getNbProcesses() == 0)
	{
	  nope::log::Log(Debug) << "Not any processes left...";
	  return (true);
	}
    }
  else
    {
      if (stdin != -1 && FD_ISSET(stdin, &m_readfds))
	{
	  if (!std::getline(std::cin, input, '\n'))
	    {
	      stdin = -1;
	    }
	  else if (std::cin)
	    {
	      // Parse input
	      m_order.push_back(input);
	    }
	}
      
      for (std::string const &s : m_order)
      {
	OrderList list;
	  ss << s;
	  while (OrderList::parse(list, ss))
	    {
	      // Exec
	      for (size_t i = 0; i < list.size(); ++i)
		{
		  m_processes.loadbalance(list.getOrder(i));
		}
	    }
	  ss.str("");
	  m_order.erase(m_order.begin());
	}
	m_order.clear();
      // Read data coming from processes
      std::cout << m_processes.getProcesses().size() << std::endl;
      for (std::vector<Process<Comm_t>>::iterator p =
               m_processes.getProcesses().begin();
           p != m_processes.getProcesses().end();)
	{
	  bool changed = false;
	  int  readFd = p->getCommunication().getReadHandler();

	  p->isAvailable();

	  if (FD_ISSET(readFd, &m_readfds))
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
  return (false);
}

std::vector<Response> Plazza::getResponse()
{
  std::vector<Response>         resp;
  std::vector<Process<Comm_t>> &p = m_processes.getProcesses();

  for (Process<Comm_t> &p : p)
    {
      resp.emplace_back();
      p.getResponse() >> resp.back();
    }

  return (resp);
}

void Plazza::pushOrder(std::string const &order)
{
  m_order.push_back(order);
}