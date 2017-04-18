#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <iomanip>
#include "LogSink.hpp"
#include "Logger.hpp"

namespace Nope
{
  namespace Log
  {
    LogSink::LogSink(std::function<void(LogMessage const &, LogLevel)> func)
        : m_func(func)
    {
    }

    void LogSink::operator()(LogMessage const &msg, LogLevel level) const
    {
      m_func(msg, level);
    }

    //   operator LogSink::LogSink(std::function<void(LogMessage const &)>
    //   func)
    //   {
    //     return LogSink(func);
    //   }

    LogSink LogSink::makeOstream(std::ostream &os)
    {
      os << "Type\t\tTime\tMessage" << std::endl;

      return LogSink([&os](LogMessage const &msg, LogLevel level) {
	std::chrono::milliseconds time(msg.time);

	os << level << (time.count() / 1000) << "." << std::setfill('0')
	   << std::setw(3) << (time.count() % 1000) << "\t" << msg.getMessage()
	   << std::endl;
      });
    }

    LogSink LogSink::makeFile(std::string const &filename)
    {
      std::shared_ptr<std::ofstream> file =
          std::make_shared<std::ofstream>(filename.c_str());

      return LogSink([file](LogMessage const &msg, LogLevel level) {
	std::chrono::milliseconds time(msg.time);

	*file << level << (time.count() / 1000) << "." << std::setfill('0')
	      << std::setw(3) << (time.count() % 1000) << "\t"
	      << msg.getMessage() << std::endl;
      });
    }
  }
}