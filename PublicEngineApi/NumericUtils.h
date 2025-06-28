#pragma once
#include <cstdint>
#include <cmath>

#include "Err.h"

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

	static Err StringToInt(const char* string, int32_t& returnValue)
	{
		char* endParsing = nullptr;
		returnValue = std::strtol(string, &endParsing, 10);

		if (endParsing == string)
			return error_const::STRING_NOT_NUM;

		return error_const::SUCCESS;
	}
};
