#pragma once
#include "Color.h"
#include "MVector.h"

struct Vertex
{
	Vec3F Pos = {0,0,0};
	ColorRgba Color = {0,0,0,0};
	Vec2F TexCoord = {0,0};
};
