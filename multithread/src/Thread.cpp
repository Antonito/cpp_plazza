#include "Thread.hpp"
#include "ThreadError.hpp"

#if __cplusplus >= 201103L
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
      throw ThreadError("Cannot create new thread"); // TODO: adjust message?
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

#if __cplusplus < 201103L
void *Thread::__thread_entry(void (*handle)(void *), void *data)
{
  handle(data);
  pthread_exit(0);
  return (NULL);
}
#else
void *Thread::__thread_entry(std::function<void()> handle)
{
  handle();
  return (nullptr);
}
#endif
