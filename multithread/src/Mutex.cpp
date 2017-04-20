#include <cassert>
#include <exception> // TODO: rm
#include "Mutex.hpp"

Mutex::Mutex() : m_locked(false),
#if __cplusplus < 201103L
m_mut()
#else
m_mut(new std::mutex())
#endif
{
#if __cplusplus < 201103L
  // We cannot use nullptr ..
  if (pthread_mutex_init(&m_mut, NULL) == -1)
    {
      throw std::exception(); // TODO: Change
    }
#endif
}

Mutex::~Mutex()
{
#if __cplusplus < 201103L
  pthread_mutex_destroy(&m_mut);
#endif
}

bool Mutex::lock()
{
  assert(m_locked == false);
#if __cplusplus < 201103L
  if (pthread_mutex_lock(&m_mut) == -1)
    {
      return (false);
    }
#else
  m_mut->lock();
#endif
  m_locked = true;
  return (true);
}

bool Mutex::unlock()
{
  assert(m_locked == true);
#if __cplusplus < 201103L
  if (pthread_mutex_unlock(&m_mut) == -1)
    {
      return (false);
    }
#else
  m_mut->unlock();
#endif
  m_locked = false;
  return (true);
}

bool Mutex::trylock()
{
  assert(m_locked == false);
#if __cplusplus < 201103L
  if (pthread_mutex_trylock(&m_mut))
    {
      return (false);
    }
#else
  if (!m_mut->try_lock())
    {
      return (false);
    }
#endif
  m_locked = true;
  return (true);
}

bool Mutex::isLocked() const
{
  return (m_locked);
}

#if __cplusplus < 201103L
pthread_mutex_t &Mutex::getMut()
{
  return (m_mut);
}
#else
std::mutex &Mutex::getMut()
{
  return (*m_mut);
}
#endif
