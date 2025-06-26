#pragma once
#include <cstdint>

struct Counter
{
	uint16_t MaxValue;
	uint16_t CurValue = 0;

	explicit Counter(const uint16_t maxValue) : MaxValue(maxValue) { }

	void Increment() { CurValue++; CurValue %= MaxValue; }
	void Decrement() { CurValue--; CurValue %= MaxValue; }
};