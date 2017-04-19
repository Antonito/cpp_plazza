#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <cstddef>

class Worker
{
public:
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);

  Worker(size_t nbThread);
  ~Worker();

  Worker(Worker const &) = delete;
  Worker &operator=(Worker const &) = delete;

  void exec();

private:
  void setReg();
  void uncipher();
  bool uncipherXor(uint8_t key);
  bool uncipherCaesar(uint16_t key);

  Regex m_reg;
  std::string m_data;
  std::vector<std::string> m_result;
};

#endif // !WORKER_HPP_
