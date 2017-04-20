#ifndef ORDERLIST_HPP_
#define ORDERLIST_HPP_

#include <string>
#include <vector>
#include <cstddef>
#include <sstream>
#include "Order.hpp"

class OrderList
{
public:
  OrderList();
  OrderList(OrderList const &other);
  ~OrderList();
  
  OrderList &operator=(OrderList const &other);
  std::string &operator[](size_t n);

  size_t size() const;
  void addFile(std::string const &file);

  void setInfo(Information info);
  Information getInfo() const;

  Order getOrder(size_t n) const;

  static bool parse(OrderList &list, std::stringstream &input);

private:
  std::vector<std::string> m_files;
  Information m_info;
};

#endif // !ORDERLIST_HPP_ 