#include "MVector.h"

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
