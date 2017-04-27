#include <exception>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t nbThread)
    : m_pool(), m_orders(), m_mut(), m_sem(0), m_running(true), m_nbThreads(0),
      m_status()
{
  for (size_t i = 0; i < nbThread; ++i)
    {
      addThread();
    }
}

ThreadPool::~ThreadPool()
{
  stopAll();
}

ThreadPool &ThreadPool::operator=(ThreadPool &&other)
{
  if (this == &other)
    return (*this);
  m_pool = std::move(other.m_pool);
  m_orders = std::move(other.m_orders);
  m_mut = std::move(other.m_mut);
  m_sem = std::move(other.m_sem);
  m_running = other.m_running.load();
  m_nbThreads = other.m_nbThreads;
  return (*this);
}

void ThreadPool::addThread()
{
  m_pool.push_back(
      Thread(std::bind(&ThreadPool::entrypoint, this, m_nbThreads)));
  m_status.push_back(false);
  ++m_nbThreads;
}

bool ThreadPool::removeThread(size_t ndx)
{
  if (ndx > getNumberThreads())
    {
      return (false);
    }
  m_pool.erase(m_pool.begin() + static_cast<signed>(ndx));
  --m_nbThreads;
  return (true);
}

void ThreadPool::stopAll()
{
  m_running = false;
  // Notify everyone
  for (Thread &cur : m_pool)
    {
      static_cast<void>(cur);
      m_sem.post();
    }

  // Wait for everyone
  for (Thread &cur : m_pool)
    {
      cur.join();
    }
  m_pool.clear();
}

size_t ThreadPool::getNumberThreads() const
{
  return (m_pool.size());
}

size_t ThreadPool::getNumberTasks() const
{
  return (m_sem.getValue());
}

Thread &ThreadPool::operator[](size_t ndx)
{
  if (ndx > getNumberThreads())
    {
      throw std::out_of_range("Tried to access inexistant thread");
    }
  return (m_pool[ndx]);
}

std::vector<bool> const &ThreadPool::getThreadStatus() const
{
  return (m_status);
}

void ThreadPool::entrypoint(std::int32_t const id)
{
  while (m_running)
    {
      // Wait for data
      m_status[id] = false;
      m_sem.wait();
      if (m_running == false)
	{
	  break;
	}

      // Thread is now working
      m_status[id] = true;

      // Grab data
      m_mut.lock();
      std::function<void()> cur = std::move(m_orders.front());
      m_orders.pop();
      m_mut.unlock();

      // Execute
      cur();
    }
}
