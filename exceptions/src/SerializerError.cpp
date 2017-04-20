#include "SerializerError.hpp"

SerializerError::SerializerError(std::string const &msg) noexcept
    : AException("Serializer error: " + msg)
{
}

SerializerError::~SerializerError()
{
}
