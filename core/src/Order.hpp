#ifndef ORDER_HPP_
#define ORDER_HPP_

#include <string>
#include <vector>
#include <cstddef>
#include "ISerializable.hpp"

enum Information
{
  PHONE_NUMBER,
  EMAIL_ADDRESS,
  IP_ADDRESS
};

class Order : public ISerializable
{
public:
  Order();
  Order(Order const &other);
  ~Order();
  
  Order &operator=(Order const &other);
  std::string &operator[](size_t n);

  size_t size() const;
  void addFile(std::string const &file);

  void setInfo(Information info);
  Information getInfo() const;

  virtual std::unique_ptr<uint8_t[]> serialize(size_t &sizeToFill);
  virtual void deserialize(size_t size, uint8_t *data);

private:
  std::vector<std::string> m_files;
  Information m_info;
};

#endif // !ORDER_HPP_ 