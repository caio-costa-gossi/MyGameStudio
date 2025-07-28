#pragma once
#include <cstdint>

#include "MVector.h"

struct BillboardData
{
	Vec3F WorldPos;
	Vec2F Scale;
	uint32_t BillboardImageId;
};
