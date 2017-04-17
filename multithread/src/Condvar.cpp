#include <exception> // TODO: rm
#include "Condvar.hpp"

Condvar::Condvar()
{
#if __cplusplus < 201103L
  // Cannot use nullptr ...
  if (pthread_cond_init(&m_cond, NULL) == -1)
    {
      throw std::exception(); // TODO
    }
#endif
}

Condvar::~Condvar()
{
#if __cplusplus < 201103L
  pthread_cond_destroy(&m_cond);
#endif
}

bool Condvar::signal()
{
#if __cplusplus < 201103L
  if (pthread_cond_signal(&m_cond) == -1)
    {
      return (false);
    }
#else
  m_cond.notify_one();
#endif
  return (true);
}

bool Condvar::broadcast()
{
#if __cplusplus < 201103L
  if (pthread_cond_broadcast(&m_cond) == -1)
    {
      return (false);
    }
#else
  m_cond.notify_all();
#endif
  return (true);
}

bool Condvar::wait()
{
  bool ret = true;

#if __cplusplus < 201103L
  m_mut.lock();
  if (pthread_cond_wait(&m_cond, &m_mut.getMut()) == -1)
    {
      ret = false;
    }
  m_mut.unlock();
#else
  std::unique_lock<std::mutex> lock(m_mut.getMut());
  m_cond.wait(lock);
#endif
  return (ret);
}