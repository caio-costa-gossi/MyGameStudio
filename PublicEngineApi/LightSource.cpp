#include "LightSource.h"

LightSource::LightSource(Vec3F pos, const ColorRgb& color) :
	pos_(std::move(pos)), color_(color)
{
	
}

Vec3F& LightSource::GetPos()
{
	return pos_;
}

ColorRgb& LightSource::GetColor()
{
	return color_;
}

enums::LightType LightSource::GetType() const
{
	return type_;
}

Err LightSource::SetColor(const ColorRgb& color)
{
	color_ = color;
	return error_const::SUCCESS;
}

Err LightSource::SetPos(const Vec3F& pos)
{
	pos_ = pos;
	return error_const::SUCCESS;
}