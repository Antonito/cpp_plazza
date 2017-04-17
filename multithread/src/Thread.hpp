#ifndef THREAD_HPP_
#define THREAD_HPP_

#if __cplusplus < 201103L
#include <pthread.h>
#else
#include <thread>
#include <functional>
#endif
#include "Mutex.hpp"

class Thread
{
public:
#if __cplusplus >= 201103L
  template <typename F, typename... Args>
  explicit Thread(F func, Args &&... args)
      : m_handle(std::bind(func, std::forward(args)...)),
        m_thread(&Thread::__thread_entry, m_handle)
  {
  }

  Thread(Thread const &) = delete;
  Thread &operator=(Thread const &) = delete;
  Thread(Thread &&);
  Thread &operator=(Thread &&);
#else
  explicit Thread(void (*func)(void *), void *data);
#endif
  ~Thread();

  bool join();
  bool detach();

private:
  void _thread_class_entry();
#if __cplusplus < 201103L
  static void *__thread_entry(void (*handle)(void *), void *arg);
  pthread_t m_thread;
  void (*m_handle)(void *);
  void *m_arg;

#else
  static void *__thread_entry(std::function<void()> handle);
  std::function<void()>                             m_handle;
  std::thread                                       m_thread;
#endif
};

#endif // !THREAD_HPP_
