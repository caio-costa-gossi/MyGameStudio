#pragma once
#include <cstdint>

class NumericUtils
{
public:
	static uint32_t Bitmask(const uint8_t val)
	{
		if (val >= 32)
			return 0;

		return 1u << val;
	}
};
