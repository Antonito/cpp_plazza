#include <exception> // TODO: rm
#include "Semaphore.hpp"

Semaphore::Semaphore(uint32_t val)
    : m_count(val)
#if __cplusplus >= 201103L
      ,
      m_mut(new std::mutex()), m_cond(new std::condition_variable())
#endif
{
#if __cplusplus < 201103L
  if (sem_init(&m_sem, 0, val) == -1)
    {
      throw std::exception(); // TODO
    }
#endif
}

Semaphore::~Semaphore()
{
#if __cplusplus < 201103L
  sem_destroy(&m_sem);
#endif
}

bool Semaphore::wait()
{
#if __cplusplus < 201103L
  if (sem_wait(&m_sem) == -1)
    {
      return (false);
    }
#else
  std::unique_lock<std::mutex> lock(*m_mut);
  while (m_count == 0)
    {
      m_cond->wait(lock);
    }
#endif
  --m_count;
  return (true);
}

bool Semaphore::post()
{
#if __cplusplus < 201103L
  if (sem_post(&m_sem) == -1)
    {
      return (false);
    }
#else
  std::unique_lock<std::mutex> lock(*m_mut);
  m_cond->notify_one();
#endif
  ++m_count;
  return (true);
}

bool Semaphore::trywait()
{
#if __cplusplus < 201103L
  if (sem_trywait(&m_sem) == -1)
    {
      return (false);
    }
#else
  std::unique_lock<std::mutex> lock(*m_mut);
  if (m_count == 0)
    {
      return (false);
    }
#endif
  --m_count;
  return (true);
}

uint32_t Semaphore::getValue() const
{
  return (m_count);
}