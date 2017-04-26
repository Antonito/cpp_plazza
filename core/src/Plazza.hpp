#ifndef PLAZZA_HPP_
#define PLAZZA_HPP_

#include <cstdint>
#include "ProcessList.hpp"
#include "UnixSocket.hpp"

class Plazza
{
public:
  explicit Plazza(std::int32_t nbThreads);
  ~Plazza();

  void loop();

private:
  // Change communiction type here
  using Comm_t = UnixSocket;

  std::int32_t        m_nbThreads;
  ProcessList<Comm_t> m_processes;
};

#endif // !PLAZZA_HPP_