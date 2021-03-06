#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <cstddef>
#include <vector>
#include <string>
#include "Order.hpp"
#include "Regex.hpp"

class Worker
{
public:
  Worker();
  ~Worker();

  Worker(Worker const &) = delete;
  Worker &operator=(Worker const &) = delete;

  void exec(Order const &);

private:
  void setReg(Information);
  void loadFile(std::string const &);
  void fillResult();
  void uncipher();
  bool uncipherXor(uint8_t *key, size_t len);
  bool uncipherCaesar(uint8_t key);

  regex::Regex             m_reg;
  std::string              m_data;
  std::string              m_uncipherData;
  std::vector<std::string> m_result;
};

#endif // !WORKER_HPP_
