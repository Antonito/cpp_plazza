#include "CommunicationError.hpp"

CommunicationError::CommunicationError(std::string const &msg) noexcept : AException("Communication error:" + msg)
{
}

CommunicationError::~CommunicationError()
{
}
