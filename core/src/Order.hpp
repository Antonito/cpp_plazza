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
  std::string &operator[](int32_t n);

  int32_t size() const;
  void addFile(std::string const &file);

  void setInfo(Information info);
  Information getInfo() const;

  virtual std::unique_ptr<uint8_t[]> serialize(int32_t &sizeToFill);
  virtual void deserialize(int32_t size, uint32_t *data);

private:
  std::vector<std::string> m_files;
  Information m_info;
};

#endif // !ORDER_HPP_ 