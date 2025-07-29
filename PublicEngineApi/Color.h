#pragma once
#include <cstdint>

#include "MVector.h"

struct ColorRgbaInt;

struct ColorRgba
{
	float R;
	float G;
	float B;
	float A;

	ColorRgba(const float r, const float g, const float b, const float a) :
		R(r), G(g), B(b), A(a) { }
	ColorRgba() = default;

	operator ColorRgbaInt() const;
};

struct ColorRgbaInt
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	ColorRgbaInt(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) :
		R(r), G(g), B(b), A(a) { }
	ColorRgbaInt() = default;

	operator ColorRgba() const;
};

struct ColorRgb
{
	float R;
	float G;
	float B;

	ColorRgb() = default;
	ColorRgb(const float r, const float g, const float b) :
		R(r), G(g), B(b) { }

	ColorRgb(const Vec3F& vec) :
		R(vec.X), G(vec.Y), B(vec.Z) { }
};