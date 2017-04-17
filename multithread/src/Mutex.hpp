#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#if __cplusplus < 201103L
#include <pthread.h>
#else
#include <mutex>
#endif

class Mutex
{
public:
  Mutex();
  ~Mutex();
#if __cplusplus >= 201103L
  Mutex(Mutex const &) = delete;
  Mutex &operator=(Mutex const &) = delete;
#endif

  bool lock();
  bool unlock();
  bool trylock();

  bool isLocked() const;

// Cannot be const, may be changed.
#if __cplusplus < 201103L
  pthread_mutex_t &getMut();
#else
  std::mutex &getMut();
#endif

private:
  bool m_locked;
#if __cplusplus < 201103L
  pthread_mutex_t m_mut;
#else
  std::mutex  m_mut;
#endif
};

#endif // !MUTEX_HPP_