#pragma once
#include <string>
#include <iostream>

class Err
{
private:
	std::string errorMessage_;
	uint16_t errorCode_;

public:
	explicit Err(std::string errorMessage, uint16_t errorCode);

	std::string Message() const;
	uint16_t Code() const;

	friend std::ostream& operator<<(std::ostream& os, const Err& err);
};


namespace error_const
{
	const auto SUCCESS = Err("Success", 0);
	const auto GENERIC_EXCEPTION = Err("Undefined error", 1);
	const auto INVALID_FORMAT = Err("Format is invalid", 2);
}