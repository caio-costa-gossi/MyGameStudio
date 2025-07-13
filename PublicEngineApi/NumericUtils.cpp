#include "NumericUtils.h"

#include <cmath>
#include <limits>

uint32_t NumericUtils::Bitmask(const uint8_t val)
{
	if (val >= 32)
		return 0;

	return 1u << val;
}

bool NumericUtils::FloatEqual(const float a, const float b, const float epsilon)
{
	return std::fabs(a - b) < epsilon;
}

Err NumericUtils::StringToUInt32(const char* string, uint32_t& returnValue)
{

	char* endParsing = nullptr;
	returnValue = std::strtoul(string, &endParsing, 10);

	if (endParsing == string)
		return error_const::STRING_NOT_NUM;

	return error_const::SUCCESS;
}

Err NumericUtils::StringToUInt32Hex(const char* string, uint32_t& returnValue)
{
	char* endParsing = nullptr;
	returnValue = std::strtoul(string, &endParsing, 16);

	if (endParsing == string)
		return error_const::STRING_NOT_NUM;

	return error_const::SUCCESS;
}

Err NumericUtils::StringToInt32(const char* string, int32_t& returnValue)
{
	char* endParsing = nullptr;
	returnValue = std::strtol(string, &endParsing, 10);

	if (endParsing == string)
		return error_const::STRING_NOT_NUM;

	return error_const::SUCCESS;
}

Err NumericUtils::StringToUInt8(const char* string, uint8_t& returnValue)
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

int32_t NumericUtils::MapRange(int32_t input, const int32_t sourceMin, const int32_t sourceMax, const int32_t targetMin, const int32_t targetMax)
{
	if (input < sourceMin)
		input = sourceMin;
	else if (input > sourceMax)
		input = sourceMax;

	const int32_t normalized = (input - sourceMin) / (sourceMax - sourceMin);
	const int32_t result = targetMin + normalized * (targetMax - targetMin);

	return result;
}

float NumericUtils::MapRangeF(float input, const float sourceMin, const float sourceMax, const float targetMin, const float targetMax)
{
	if (input < sourceMin)
		input = sourceMin;
	else if (input > sourceMax)
		input = sourceMax;

	const float normalized = (input - sourceMin) / (sourceMax - sourceMin);
	const float result = targetMin + normalized * (targetMax - targetMin);

	return result;
}

float NumericUtils::Normalize(int32_t input, const int32_t sourceMin, const int32_t sourceMax)
{
	if (input < sourceMin)
		input = sourceMin;
	else if (input > sourceMax)
		input = sourceMax;

	const float normalized = static_cast<float>(input - sourceMin) / static_cast<float>(sourceMax - sourceMin);

	return normalized;
}

float NumericUtils::NormalizeF(float input, const float sourceMin, const float sourceMax)
{
	if (input < sourceMin)
		input = sourceMin;
	else if (input > sourceMax)
		input = sourceMax;

	const float normalized = (input - sourceMin) / (sourceMax - sourceMin);

	return normalized;
}