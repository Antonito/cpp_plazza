#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#if __cplusplus < 201103L
#error "ThreadPool requires C++11."
#endif

#include <atomic>
#include <vector>
#include <queue>
#include "Thread.hpp"
#include "Semaphore.hpp"
#include "Mutex.hpp"

class ThreadPool
{
public:
  ThreadPool(std::size_t nbThread = 0);
  ~ThreadPool();
  ThreadPool(ThreadPool const &) = delete;
  ThreadPool &operator=(ThreadPool const &) = delete;
  ThreadPool(ThreadPool &&) = default;
  ThreadPool &operator=(ThreadPool &&);

  // Thread controls
  void addThread();
  bool removeThread(std::size_t ndx);
  void stopAll();

  template <typename F, typename... Args>
  bool execute(F func, Args &&... args)
  {
    m_mut.lock();
    m_orders.push(std::bind(func, std::forward<Args>(args)...));
    m_mut.unlock();
    m_sem.post();
    return (true);
  }

  std::size_t              getNumberThreads() const;
  std::size_t              getNumberTasks() const;
  std::vector<bool> const &getThreadStatus() const;
  Thread &operator[](std::size_t ndx);

private:
  void entrypoint(std::int32_t const);

  std::vector<Thread>               m_pool;
  std::queue<std::function<void()>> m_orders;
  Mutex                             m_mut;
  Semaphore                         m_sem;
  std::atomic<bool>                 m_running;
  std::int32_t                      m_nbThreads;
  std::vector<bool>                 m_status;
};

#endif // !THREADPOOL_HPP_
