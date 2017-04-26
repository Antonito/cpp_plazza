#include <exception>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t nbThread)
    : m_pool(), m_orders(), m_mut(), m_sem(0), m_running(true)
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
  return (*this);
}

void ThreadPool::addThread()
{
  m_pool.push_back(Thread(std::bind(&ThreadPool::entrypoint, this)));
}

bool ThreadPool::removeThread(size_t ndx)
{
  if (ndx > getNumberThreads())
    {
      return (false);
    }
  m_pool.erase(m_pool.begin() + static_cast<signed>(ndx));
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
      throw std::out_of_range(
          "Tried to access inexistant thread"); // TODO: adjust message?
    }
  return (m_pool[ndx]);
}

void ThreadPool::entrypoint()
{
  while (m_running)
    {
      // Wait for data
      m_sem.wait();
      if (m_running == false)
	{
	  break;
	}

      // Grab data
      m_mut.lock();
      std::function<void()> cur = std::move(m_orders.front());
      m_orders.pop();
      m_mut.unlock();

      // Execute
      cur();
    }
}
