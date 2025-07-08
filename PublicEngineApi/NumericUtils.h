#pragma once
#include <cstdint>
#include <cmath>
#include <limits>

#include "Err.h"

#undef max

class NumericUtils
{
public:
	static uint32_t Bitmask(const uint8_t val)
	{
		if (val >= 32)
			return 0;

		return 1u << val;
	}

	static bool FloatEqual(const float a, const float b, const float epsilon)
	{
		return std::fabs(a - b) < epsilon;
	}

	static Err StringToUInt32(const char* string, uint32_t& returnValue)
	{
		char* endParsing = nullptr;
		returnValue = std::strtoul(string, &endParsing, 10);

		if (endParsing == string)
			return error_const::STRING_NOT_NUM;

		return error_const::SUCCESS;
	}

	static Err StringToInt32(const char* string, int32_t& returnValue)
	{
		char* endParsing = nullptr;
		returnValue = std::strtol(string, &endParsing, 10);

		if (endParsing == string)
			return error_const::STRING_NOT_NUM;

		return error_const::SUCCESS;
	}

	static Err StringToUInt8(const char* string, uint8_t& returnValue)
	{
		char* endParsing = nullptr;
		const int32_t val = std::strtol(string, &endParsing, 10);

		if (endParsing == string)
			return error_const::STRING_NOT_NUM;

		if (val < 0 || val > std::numeric_limits<uint8_t>::max())
			return error_const::INTEGER_OUT_OF_BOUNDS;

		returnValue = static_cast<uint8_t>(val);
		return error_const::SUCCESS;
	}
};
