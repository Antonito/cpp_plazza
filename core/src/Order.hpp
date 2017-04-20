#ifndef ORDER_HPP_
#define ORDER_HPP_

#include <string>
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
  Order(std::string const &file, Information type);
  Order(Order const &other);
  virtual ~Order();

  Order &operator=(Order const &other);

  virtual std::unique_ptr<uint8_t[]> serialize(size_t &sizeToFill) const;
  virtual void deserialize(size_t size, uint8_t *data);

  std::string const &getFile() const;
  Information getInfo() const;

private:
  std::string m_file;
  Information m_info;
};

#endif // !ORDER_HPP_