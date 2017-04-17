#ifndef CONDVAR_HPP_
#define CONDVAR_HPP_

#if __cplusplus < 201103L
#include <pthread.h>
#else
#include <condition_variable>
#endif
#include "Mutex.hpp"

class Condvar
{
public:
  Condvar();
  ~Condvar();
#if __cplusplus >= 201103L
  Condvar(Condvar const &) = delete;
  Condvar &operator=(Condvar const &) = delete;
#endif

  bool signal();
  bool broadcast();
  bool wait();

private:
#if __cplusplus < 201103L
  pthread_cond_t m_cond;
#else
  std::condition_variable m_cond;
#endif
  Mutex m_mut;
};

#endif // !CONDVAR_HPP_