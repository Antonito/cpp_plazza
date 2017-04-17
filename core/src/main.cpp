#include <iostream>
#include "ProcessList.hpp"
#include <unistd.h>

int main(int ac, char **av)
{
  if (ac == 2)
    {
      int32_t thread_nb = static_cast<int32_t>(std::strtol(av[1], NULL, 0));

      if (thread_nb > 0)
	{
#if 0
	  Process proc(static_cast<size_t>(thread_nb));

	  proc.run();
	  proc.wait();
#endif

	  // Launch plazza here
	  return (0);
	}
    }
  std::cout << *av << " thread_number" << std::endl;
  return (1);
}
