#include <iostream>
#include <chrono>
#include "ThreadPool.hpp"

void print_number(int a)
{
  std::cout << "[ " << std::this_thread::get_id() << "] Number is " << a
            << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main()
{
  ThreadPool pool;

  // Check hardware cores
  std::cout << "We have " << std::thread::hardware_concurrency() << " cores."
            << std::endl;
  for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
      pool.addThread();
    }
  std::cout << "There are " << pool.getNumberThreads() << " threads."
            << std::endl;

  // Execute actions
  for (size_t i = 0; i < 15; ++i)
    {
      pool.execute(print_number, i);
    }

  // Sleep a bit
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return (0);
}
