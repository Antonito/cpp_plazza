#ifndef PLAZZA_HPP_
#define PLAZZA_HPP_

#include <cstdint>
#include "ProcessList.hpp"
#include "InternetSocket.hpp"

class Plazza
{
public:
  explicit Plazza(std::int32_t nbThreads);
  ~Plazza();

  void loop();

private:
  std::int32_t                m_nbThreads;
  ProcessList<InternetSocket> m_processes;
};

#endif // !PLAZZA_HPP_