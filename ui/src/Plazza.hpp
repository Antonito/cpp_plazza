#ifndef PLAZZA_HPP_
#define PLAZZA_HPP_

#include <cstdint>
#include <vector>
#include <string>
#include "ProcessList.hpp"
#include "UnixSocket.hpp"
#include "InternetSocket.hpp"
#include "Response.hpp"
#include "OrderList.hpp"

class Plazza
{
public:
  explicit Plazza(std::int32_t nbThreads);
  ~Plazza();

  void                  update();
  bool                  loop();
  std::vector<Response> getResponse();
  void pushOrder(std::string const &order);

private:
  // Change communiction type here
  using Comm_t = InternetSocket;

  std::int32_t             m_nbThreads;
  ProcessList<Comm_t>      m_processes;
  bool                     m_finished;
  std::vector<std::string> m_order;
  OrderList                m_orderList;
  fd_set                   m_readfds;
};

#endif // !PLAZZA_HPP_