#include <exception>
#include "Thread.hpp"

#if __cplusplus >= 201103L
template <typename F, typename... Args>
Thread::Thread(F func, Args &&... args)
    : m_handle(std::bind(func, std::forward(args)...)),
      m_thread(&Thread::__thread_entry)
{
}

Thread::Thread(Thread &&other)
    : m_handle(other.m_handle), m_thread(std::move(other.m_thread))
{
}

Thread &Thread::operator=(Thread &&other)
{
  if (this != &other)
    {
      m_handle = other.m_handle;
      m_thread = std::move(other.m_thread);
    }
  return (*this);
}

#else
Thread::Thread(void (*func)(void *), void *data) : m_handle(func), m_arg(data)
{
  if (pthread_create(&m_thread, NULL, &Thread::__thread_entry, this) == -1)
    {
      throw std::exception(); // TODO
    }
}
#endif

Thread::~Thread()
{
}

bool Thread::join()
{
#if __cplusplus < 201103L
  if (pthread_join(m_thread, NULL) == -1)
    {
      return (false);
    }
#else
  m_thread.join();
#endif
  return (true);
}

bool Thread::detach()
{
#if __cplusplus < 201103L
  if (pthread_detach(m_thread) == -1)
    {
      return (false);
    }
#else
  m_thread.detach();
#endif
  return (true);
}

void Thread::_thread_class_entry()
{
#if __cplusplus < 201103L
  m_handle(m_arg);
#else
  m_handle();
#endif
}

void *Thread::__thread_entry(void *data)
{
  Thread *inst = reinterpret_cast<Thread *>(data);

  inst->_thread_class_entry();
#if __cplusplus < 201103L
  pthread_exit(0);
  return (NULL);
#else
  return (nullptr);
#endif
}