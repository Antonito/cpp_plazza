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
  ThreadPool();
  ~ThreadPool();
  ThreadPool(ThreadPool const &) = delete;
  ThreadPool &operator=(ThreadPool const &) = delete;

  // Thread controls
  void addThread();
  bool removeThread(size_t ndx);
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

  size_t getNumberThreads() const;
  Thread &operator[](size_t ndx);

private:
  void entrypoint();

  std::vector<Thread>               m_pool;
  std::queue<std::function<void()>> m_orders;
  Mutex                             m_mut;
  Semaphore                         m_sem;
  std::atomic<bool>                 m_running;
};

#endif // !THREADPOOL_HPP_
