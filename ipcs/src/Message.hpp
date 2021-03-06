#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <memory>
#include <type_traits>
#include <cstring>
#include "IMessage.hpp"
#include "ISerializable.hpp"

template <typename T>
class Message : public IMessage
{
  // Small check
  static_assert(std::is_base_of<ISerializable, T>(),
                "Type is not an ISerializable");

public:
  Message() : m_size(0), m_data()
  {
  }

  Message(Message const &other)
      : m_size(other.m_size), m_data(new uint8_t[m_size])
  {
    std::memcpy(m_data.get(), other.m_data.get(), m_size);
  }

  Message(Message &&other)
      : m_size(other.m_size), m_data(std::move(other.m_data))
  {
    other.m_size = 0;
  }

  Message(T const &obj)
  {
    this->fillData(obj);
  }

  virtual ~Message()
  {
  }

  Message &operator=(Message const &other)
  {
    if (this == &other)
      return (*this);
    m_size = other.m_size;
    m_data = std::make_unique<uint8_t[]>(m_size);
    std::memcpy(m_data.get(), other.m_data.get(), m_size);
    return (*this);
  }

  Message &operator=(Message &&other)
  {
    if (this == &other)
      return (*this);
    m_size = other.m_size;
    m_data = std::move(m_data);
    other.m_size = 0;
    return (*this);
  }

  Message &operator=(T const &obj)
  {
    this->fillData(obj);
  }

  void fillMessage(T const &obj)
  {
    m_data = obj.serialize(m_size);
  }

  void getMessage(T &obj)
  {
    obj.deserialize(m_size, m_data.get());
  }

  Message &operator<<(T const &obj)
  {
    this->fillMessage(obj);
    return (*this);
  }

  Message &operator>>(T &obj)
  {
    this->getMessage(obj);
    return (*this);
  }

  virtual size_t getSize() const
  {
    return (m_size);
  }

  virtual uint8_t *getData() const
  {
    return (m_data.get());
  }

  virtual void setData(size_t size, std::unique_ptr<uint8_t[]> data)
  {
    m_size = size;
    m_data = std::move(data);
  }

private:
  size_t                     m_size;
  std::unique_ptr<uint8_t[]> m_data;
};

#endif // !MESSAGE_HPP_