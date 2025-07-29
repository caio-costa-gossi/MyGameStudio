#include "MVector.h"

#include "Color.h"

Vec2U64::operator Vec3U64() const
{
	return { X,Y,0 };
}

Vec2I64::operator Vec3I64() const
{
	return { X,Y,0 };
}

Vec2F::operator Vec3F() const
{
	return { X,Y,0 };
}

Vec3F::Vec3F(const ColorRgb& color)
{
	X = color.R;
	Y = color.G;
	Z = color.B;
}
