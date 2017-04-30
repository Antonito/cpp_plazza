#include "window.h"
#include <QApplication>
#include "wprocess.h"

int main(int ac, char *av[])
{
  if (ac == 2)
    {

      int32_t threadNb = static_cast<int32_t>(std::strtol(av[1], NULL, 0));

      if (threadNb > 0)
	{
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

      QApplication a(ac, av);
	  Window       window(threadNb);

      WProcess::threadPerProcess = threadNb;

	  window.show();
	  
	  int ret = a.exec();

	  nope::log::Log(Info)
	      << "Leaving main process. All children should be killed by now.";
	  return (ret);
	}
    }
  nope::log::Log(Error) << "Wrong number of arguments.";
  std::cout << *av << " thread_number" << std::endl;
  return (1);
}
