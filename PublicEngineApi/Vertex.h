#pragma once
#include "Color.h"
#include "MVector.h"

struct Vertex
{
	Vec3F Pos = { 0,0,0 };
	Vec3F Normal = { 0,0,0 };
	ColorRgba Color = { 0,0,0,0 };

	Vec2F BaseColorTexCoord = { 0,0 };
	Vec2F NormalTexCoord = { 0,0 };
	Vec2F MetallicRoughnessTexCoord = { 0,0 };
	Vec2F OcclusionTexCoord = { 0,0 };
	Vec2F EmissiveTexCoord = { 0,0 };
};
