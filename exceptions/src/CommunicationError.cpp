#include "CommunicationError.hpp"

CommunicationError::CommunicationError(std::string const &msg) : AException("Communication error:" + msg)
{
}

CommunicationError::~CommunicationError()
{
}
