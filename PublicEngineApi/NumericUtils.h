#pragma once
#include <cstdint>

class NumericUtils
{
public:
	static uint8_t Bitmask(const uint8_t val)
	{
		if (val >= 8)
			return 0;

		return static_cast<uint8_t>(1u << val);
	}
};
