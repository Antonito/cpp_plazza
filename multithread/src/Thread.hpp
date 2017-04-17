#ifndef THREAD_HPP_
#define THREAD_HPP_

#if __cplusplus < 201103L
#include <pthread.h>
#else
#include <thread>
#include <functional>
#endif

class Thread
{
public:
#if __cplusplus >= 201103L
  template <typename F, typename... Args>
  explicit Thread(F func, Args &&... args);
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
  void         _thread_class_entry();
  static void *__thread_entry(void *data);
#if __cplusplus < 201103L
  pthread_t m_thread;
  void (*m_handle)(void *);
  void *m_arg;

#else
  std::function<void()> m_handle;
  std::thread           m_thread;
#endif
};

#endif // !THREAD_HPP_
