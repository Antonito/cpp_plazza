#include <exception> // TODO: rm
#include "ThreadPool.hpp"
#include <iostream> // TODO: rm
ThreadPool::ThreadPool() : m_sem(0), m_running(true)
{
}

ThreadPool::~ThreadPool()
{
  stopAll();
}

void hello()
{
  std::cout << "Wow !" << std::endl;
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

Thread &ThreadPool::operator[](size_t ndx)
{
  if (ndx > getNumberThreads())
    {
      throw std::exception(); // TODO
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