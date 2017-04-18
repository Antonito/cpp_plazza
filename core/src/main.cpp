#include <unistd.h>
#include <iostream>
#include "ProcessList.hpp"
#include "Logger.hpp"

int main(int ac, char **av)
{
  if (ac == 2)
    {
      int32_t thread_nb = static_cast<int32_t>(std::strtol(av[1], NULL, 0));

      if (thread_nb > 0)
	{
	  ProcessList processes(static_cast<size_t>(thread_nb));

	  Nope::Log::Logger::start();
#if defined(DEBUG)
	  Nope::Log::Logger::logLevel = Nope::Log::LogLevel::LOG_DEBUG;
	  Nope::Log::Debug << "Starting log";
#else
	  Nope::log::Logger::logLevel = Nope::Log::LogLevel::LOG_INFO;
#endif
	  // Launch plazza here
	  while (1)
	    {
	      std::string input;

	      std::getline(std::cin, input, '\n');
	      if (!std::cin)
		{
		  break;
		}
	      processes.checkTimeout();
	      // Parse input
	      // Exec
	      // processes.loadbalance();
	      // Show result
	    }
#if defined(DEBUG)
	  Nope::Log::Debug << "Leaving log";
#endif
	  return (0);
	}
    }
  std::cout << *av << " thread_number" << std::endl;
  return (1);
}
