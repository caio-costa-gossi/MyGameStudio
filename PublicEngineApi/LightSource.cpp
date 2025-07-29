#include "LightSource.h"

LightSource::LightSource(Vec3F pos, const ColorRgb& color, const float intensity) :
	pos_(std::move(pos)), color_(color), intensity_(intensity)
{
	
}

const Vec3F& LightSource::GetPos() const
{
	return pos_;
}

ColorRgb LightSource::GetColor() const
{
	return color_;
}

float LightSource::GetIntensity() const
{
	return intensity_;
}
