#pragma once
#include <cstdint>

#include "Err.h"
#include "MVector.h"

#undef max

class NumericUtils
{
public:
	static uint32_t Bitmask(const uint8_t val);

	static bool FloatEqual(float a, float b, float epsilon);
	static float QuickRSqrt(float number);

	static Err StringToUInt32(const char* string, uint32_t& returnValue);
	static Err StringToUInt32Hex(const char* string, uint32_t& returnValue);
	static Err StringToInt32(const char* string, int32_t& returnValue);
	static Err StringToUInt8(const char* string, uint8_t& returnValue);

	static int32_t MapRange(int32_t input, int32_t sourceMin, int32_t sourceMax, int32_t targetMin, int32_t targetMax);
	static float MapRangeF(float input, float sourceMin, float sourceMax, float targetMin, float targetMax);
	static float Normalize(int32_t input, int32_t sourceMin, int32_t sourceMax);
	static float NormalizeF(float input, float sourceMin, float sourceMax);

	static Vec3F Normalize(const Vec3F& vector);
	static Vec3F CrossProduct(const Vec3F& a, const Vec3F& b);
};
