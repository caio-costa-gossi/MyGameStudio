#include "Color.h"

#include "NumericUtils.h"

constexpr uint8_t COLOR_MIN_VALUE = 0;
constexpr uint8_t COLOR_MAX_VALUE = 255;
constexpr float COLOR_MAX_VALUE_F = 255.0f;

ColorRgba::operator ColorRgbaInt() const
{
	const uint8_t intR = static_cast<uint8_t>(R * COLOR_MAX_VALUE_F);
	const uint8_t intG = static_cast<uint8_t>(G * COLOR_MAX_VALUE_F);
	const uint8_t intB = static_cast<uint8_t>(B * COLOR_MAX_VALUE_F);
	const uint8_t intA = static_cast<uint8_t>(A * COLOR_MAX_VALUE_F);

	return { intR, intG, intB, intA };
}

ColorRgbaInt::operator ColorRgba() const
{
	const float floatR = NumericUtils::Normalize(R, COLOR_MIN_VALUE, COLOR_MAX_VALUE);
	const float floatG = NumericUtils::Normalize(G, COLOR_MIN_VALUE, COLOR_MAX_VALUE);
	const float floatB = NumericUtils::Normalize(B, COLOR_MIN_VALUE, COLOR_MAX_VALUE);
	const float floatA = NumericUtils::Normalize(A, COLOR_MIN_VALUE, COLOR_MAX_VALUE);

	return { floatR, floatG, floatB, floatA };
}
