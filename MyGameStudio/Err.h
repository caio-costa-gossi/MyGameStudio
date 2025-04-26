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
	const auto SETTING_MISSING = Err("A configuration needed has not been set", 3);
	const auto INVALID_PARAMETERS = Err("Invalid parameters", 4);
	const auto REDUNDANT_CALL = Err("This operation has already been done", 5);
	const auto DB_CLOSED = Err("The operation cannot proceed, the database is closed", 6);
	const auto INTEGER_OUT_OF_BOUNDS = Err("Integer is out of bounds", 7);

	constexpr auto ZIP_ERROR_CODE = 11;
}