#ifndef SEMAPHORE_HPP_
#define SEMAPHORE_HPP_

#if __cplusplus < 201103L
#include <stdint.h>
#include <semaphore.h>
#else
#include <cstdint>
#include <mutex>
#include <condition_variable>
#endif

class Semaphore
{
public:
  explicit Semaphore(uint32_t val);
  ~Semaphore();
#if __cplusplus >= 201103L
  Semaphore(Semaphore const &) = delete;
  Semaphore &operator=(Semaphore const &) = delete;
#endif

  bool wait();
  bool post();
  bool trywait();

  uint32_t getValue() const;

private:
  uint32_t m_count;
#if __cplusplus < 201103L
  sem_t m_sem;
#else
  std::mutex              m_mut;
  std::condition_variable m_cond;
#endif
};

#endif // !SEMAPHORE_HPP_