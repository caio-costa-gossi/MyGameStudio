#pragma once
#include <cstdint>

struct Vec2U64
{
	uint64_t X;
	uint64_t Y;

	Vec2U64() : X(0), Y(0) { }
	Vec2U64(const uint64_t x, const uint64_t y) : X(x), Y(y) { }
};

struct Vec3U64
{
	uint64_t X;
	uint64_t Y;
	uint64_t Z;

	Vec3U64() : X(0), Y(0), Z(0) { }
	Vec3U64(const uint64_t x, const uint64_t y, const uint64_t z) : X(x), Y(y), Z(z) { }
};

struct Vec2F
{
	float X;
	float Y;

	Vec2F() : X(0), Y(0) { }
	Vec2F(const float x, const float y) : X(x), Y(y) { }
};

struct Vec3F
{
	float X;
	float Y;
	float Z;

	Vec3F() : X(0), Y(0), Z(0) { }
	Vec3F(const float x, const float y, const float z) : X(x), Y(y), Z(z) { }
};