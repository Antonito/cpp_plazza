#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include "LogMessage.hpp"
#include "LogSink.hpp"

namespace Nope
{
  namespace Log
  {
    enum class LogLevel : int
    {
      LOG_TRACE,
#ifdef DEBUG
      LOG_DEBUG,
#endif
      LOG_INFO,
      LOG_WARNING,
      LOG_ERROR
    };

    class Logger
    {
    public:
      Logger(LogLevel level = LogLevel::LOG_INFO);
      ~Logger() = default;

      void addSink(LogSink const &s);

#ifdef DEBUG
      LogMessage operator()(std::string &&file, size_t line);
#endif

      template <typename T>
      LogMessage operator<<(T const &e)
      {
	LogMessage msg(this);

	msg << e;
	return msg;
      }

      void flush(LogMessage const &) const;

      static void start();

      static LogLevel logLevel;
      static const std::chrono::time_point<std::chrono::high_resolution_clock,
                                           std::chrono::milliseconds>
          startTime;

    private:
      std::vector<LogSink> m_sinks;
      LogLevel             m_level;
    };

    std::ostream &operator<<(std::ostream &os, LogLevel level);

    extern Logger Trace;
#ifdef DEBUG
    extern Logger Debug;
#endif
    extern Logger Info;
    extern Logger Warning;
    extern Logger Error;
  }
}

#ifdef DEBUG
#define Log(logger) logger(__FILE__, __LINE__)
#else
#define Log(logger) logger
#endif

#endif // !LOGGER_HPP_