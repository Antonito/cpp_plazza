#include "LogSink.hpp"
#include "Logger.hpp"

namespace nope
{
  namespace log
  {
    // Do not touch
    std::chrono::time_point<std::chrono::high_resolution_clock,
                            std::chrono::milliseconds> const
        Logger::startTime =
            std::chrono::time_point_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now());

    LogLevel Logger::logLevel = LogLevel::LOG_INFO;

    Logger Trace(LogLevel::LOG_TRACE);
#ifdef DEBUG
    Logger Debug(LogLevel::LOG_DEBUG);
#else
    EmptyLogger Debug(LogLevel::LOG_DEBUG);
#endif
    Logger Info(LogLevel::LOG_INFO);
    Logger Warning(LogLevel::LOG_WARNING);
    Logger Error(LogLevel::LOG_ERROR);

    Logger::Logger(LogLevel level) : m_level(level)
    {
    }

    void Logger::addSink(LogSink const &s)
    {
      m_sinks.push_back(s);
    }

#ifdef DEBUG
    LogMessage Logger::operator()(std::string &&file, size_t line)
    {
      return LogMessage(this, std::move(file), line);
    }
#endif

    void Logger::flush(LogMessage const &msg) const
    {
      if (m_level >= logLevel)
	{
	  for (auto &s : m_sinks)
	    {
	      s(msg, m_level);
	    }
	}
    }

    void Logger::start()
    {
      LogSink file = LogSink::makeFile("log/plazza.log");

      Trace.addSink(file);
#ifdef DEBUG
      Debug.addSink(file);
#endif
      Info.addSink(file);
      Warning.addSink(file);
      Error.addSink(file);
    }

    std::ostream &operator<<(std::ostream &os, LogLevel level)
    {
      switch (level)
	{
	case LogLevel::LOG_TRACE:
	  os << "{{TRACE}}";
	  break;
#ifdef DEBUG
	case LogLevel::LOG_DEBUG:
	  os << "[DEBUG]\t";
	  break;
#endif
	case LogLevel::LOG_INFO:
	  os << "[INFO]\t";
	  break;
	case LogLevel::LOG_WARNING:
	  os << "**WARNING**";
	  break;
	case LogLevel::LOG_ERROR:
	  os << "!!ERROR!!";
	  break;
	default:
	  os << "? Unknown ?";
	}
      os << "\t";
      return os;
    }
  }
}