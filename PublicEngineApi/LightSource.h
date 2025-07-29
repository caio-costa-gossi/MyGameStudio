#pragma once
#include "MVector.h"
#include "Color.h"

class LightSource
{
private:
	Vec3F pos_;
	ColorRgb color_;
	float intensity_;

public:
	LightSource() = default;
	LightSource(Vec3F pos, const ColorRgb& color, float intensity);

	const Vec3F& GetPos() const;
	ColorRgb GetColor() const;
	float GetIntensity() const;
};
