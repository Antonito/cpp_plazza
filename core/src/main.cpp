#include <csignal>
#include <iostream>
#include <sstream>
#include "ProcessList.hpp"
#include "Logger.hpp"
#include "Order.hpp"
#include "OrderList.hpp"
#include "UnixSocket.hpp"

int main(int ac, char **av)
{
  if (ac == 2)
    {
      int32_t thread_nb = static_cast<int32_t>(std::strtol(av[1], NULL, 0));

      if (thread_nb > 0)
	{
	  // You can change communication type here
	  ProcessList<UnixSocket> processes(static_cast<size_t>(thread_nb));

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
		  struct timeval tm;

		  tm.tv_sec = 0;
		  tm.tv_usec = 5;
		  FD_ZERO(&readfds);
		  FD_SET(0, &readfds);
		  ret = select(0 + 1, &readfds, nullptr, nullptr, &tm);
		}
	      while (ret == -1 && errno == EINTR);
	      if (ret == -1)
		{
		  break;
		}
	      else if (ret > 0)
		{
		  if (FD_ISSET(0, &readfds))
		    {
		      std::getline(std::cin, input, '\n');
		      if (!std::cin)
			{
			  // All data was sent to processes
			  break;
			}

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
	    }
	  processes.wait();
	  nope::log::Log(Debug) << "Leaving log";
	  return (0);
	}
    }
  std::cout << *av << " thread_number" << std::endl;
  return (1);
}
