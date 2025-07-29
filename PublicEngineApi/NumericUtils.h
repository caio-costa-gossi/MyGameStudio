#pragma once
#include <cstdint>

#include "Err.h"
#include "MVector.h"
#include "Transform.h"
#include "Transform3.h"

#undef max

namespace NumericUtils
{
	// Constants
	constexpr float PI = 3.14159265f;

	// Bitmask Functions
	uint32_t Bitmask(const uint8_t val);

	// Conversion Functions
	Err StringToUInt32(const char* string, uint32_t& returnValue);
	Err StringToUInt32Hex(const char* string, uint32_t& returnValue);
	Err StringToInt32(const char* string, int32_t& returnValue);
	Err StringToUInt8(const char* string, uint8_t& returnValue);

	// General Math Functions
	int32_t MapRange(int32_t input, int32_t sourceMin, int32_t sourceMax, int32_t targetMin, int32_t targetMax);
	float MapRangeF(float input, float sourceMin, float sourceMax, float targetMin, float targetMax);
	float Normalize(int32_t input, int32_t sourceMin, int32_t sourceMax);
	float NormalizeF(float input, float sourceMin, float sourceMax);

	bool FloatEqual(float a, float b, float epsilon);
	float QuickRSqrt(float number);

	// Vector Operations
	Vec3F Normalize(const Vec3F& vector);
	Vec3F CrossProduct(const Vec3F& a, const Vec3F& b);

	// Matrix Operations
	Transform3 CalculateNormalMatrix(const Transform& modelMatrix);

	// Trigonometric Functions
	float Radians(float degrees);
	float Degrees(float radians);
};
