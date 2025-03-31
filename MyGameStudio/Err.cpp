#include "Err.h"

Err::Err(std::string errorMessage, uint16_t errorCode): errorMessage_(std::move(errorMessage)), errorCode_(errorCode)
{
	
}

std::string Err::Message() const
{
	return this->errorMessage_;
}

uint16_t Err::Code() const
{
	return this->errorCode_;
}

std::ostream& operator<<(std::ostream& os, const Err& err)
{
	os << err.errorMessage_ << " (Error code: " << err.errorCode_ << ")";
	return os;
}