#include <csignal>
#include <iostream>
#include <sstream>
#include "ProcessList.hpp"
#include "Logger.hpp"
#include "Order.hpp"
#include "OrderList.hpp"
#include "InternetSocket.hpp"

int main(int ac, char **av)
{
  if (ac == 2)
    {
      using ProcessList_t = ProcessList<InternetSocket>;
      using Process_t = Process<InternetSocket>;

      int32_t thread_nb = static_cast<int32_t>(std::strtol(av[1], NULL, 0));

      if (thread_nb > 0)
	{
	  // You can change communication type here
	  ProcessList_t processes(static_cast<size_t>(thread_nb));

	  // Starts logger
	  nope::log::Logger::start();
#if defined(DEBUG)
	  nope::log::Logger::logLevel = nope::log::LogLevel::LOG_DEBUG;
	  nope::log::Log(Debug) << "Starting log";
#else
	  nope::log::Logger::logLevel = nope::log::LogLevel::LOG_INFO;
#endif

	  std::signal(SIGPIPE, SIG_IGN);
	  nope::log::Log(Debug) << "Ignoring SIGPIPE signals";
	  nope::log::Log(Info) << "Starting processing.";
	  // Launch plazza here
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

		  maxFd = STDIN_FILENO;

		  // Set timer
		  tm.tv_sec = 2;
		  tm.tv_usec = 0;

		  FD_ZERO(&readfds);

		  // Add stdin
		  FD_SET(STDIN_FILENO, &readfds);

		  // Add process file descriptors
		  for (Process_t const &p : processes.getProcesses())
		    {
		      int readFd = p.getCommunication().getReadHandler();

		      FD_SET(readFd, &readfds);
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
		  nope::log::Log(Error) << "select() failed";
		  break;
		}
	      else if (ret == 0)
		{
		  // If there are no more processes, exit
		  nope::log::Log(Debug) << "Main process timed out";
		  if (processes.getNbProcesses() == 0)
		    {
		      nope::log::Log(Debug) << "Not any processes left...";
		      break;
		    }
		}
	      else if (ret > 0)
		{
		  if (FD_ISSET(STDIN_FILENO, &readfds))
		    {
		      std::getline(std::cin, input, '\n');
		      if (std::cin)
			{
			  // Parse input
			  ss << input;
			  while (OrderList::parse(orderList, ss))
			    {
			      // Exec
			      for (size_t i = 0; i < orderList.size(); ++i)
				{
				  processes.loadbalance(orderList.getOrder(i));
				}
			    }
			}
		    }
		  if (processes.getNbProcesses() == 0)
		    {
		      nope::log::Log(Debug) << "Not any processes left...";
		      break;
		    }

		  // Read data coming from processes
		  for (std::vector<Process_t>::iterator p =
		           processes.getProcesses().begin();
		       p != processes.getProcesses().end();)
		    {
		      bool changed = false;
		      int  readFd = p->getCommunication().getReadHandler();

		      if (FD_ISSET(readFd, &readfds))
			{
			  if (p->getCommunication().read(p->getResponse()) ==
			      false)
			    {
			      changed = true;
			      processes.removeProcess(*p);
			    }
			}
		      if (changed == false)
			{
			  ++p;
			}
		    }
		}
	    }
	  nope::log::Log(Debug) << "Leaving log";
	  processes.wait();
	  nope::log::Log(Info) << "Processing over.";
	  return (0);
	}
    }
  nope::log::Log(Error) << "Wrong number of arguments.";
  std::cout << *av << " thread_number" << std::endl;
  return (1);
}
