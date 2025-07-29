#pragma once
#include "MVector.h"
#include "Color.h"

struct LightSource
{
	Vec3F Pos = { };
	ColorRgb Color = { };
	float Intensity = 0;

	LightSource() = default;
	LightSource(Vec3F pos, const ColorRgb& color, const float intensity) :
		Pos(std::move(pos)), Color(color), Intensity(intensity) { }
};
