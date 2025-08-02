#pragma once
#include "Primitive.h"
#include "RenderManager.h"

struct Line final : Primitive
{
	Vec3F StartPoint;
	Vec3F EndPoint;

	Line(Vec3F startPoint, Vec3F endPoint, const ColorRgb color) :
		StartPoint(std::move(startPoint)), EndPoint(std::move(endPoint))
	{
		Color = color;
	}

	void RequestDraw() override
	{
		RenderManager::RequestRender({})
	}
};
