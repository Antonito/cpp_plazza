#include "ThreadError.hpp"

ThreadError::ThreadError(std::string const &msg) noexcept
    : AException("Thread error:" + msg)
{
}

ThreadError::~ThreadError()
{
}