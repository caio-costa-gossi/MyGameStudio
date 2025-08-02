#pragma once
#include "MVector.h"
#include "Color.h"
#include "Transform.h"

struct Primitive
{
	Transform TransformMatrix;
	ColorRgb Color;

	Primitive(const Vec3F& pos, const float rotationDegrees, const Vec3F& rotationAxis, const Vec3F& scale, const ColorRgb color) :
		Color(color)
	{
		TransformMatrix = Transform(pos, rotationDegrees, rotationAxis, scale);
	}

	virtual void RequestDraw() = 0;
	virtual ~Primitive() = default;
};
