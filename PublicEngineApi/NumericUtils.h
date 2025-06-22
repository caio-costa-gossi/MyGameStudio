#pragma once
#include <cstdint>
#include <cmath>

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
};
