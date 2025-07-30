#pragma once
#include <cstdint>

#include "MVector.h"

struct Material
{
	int32_t BaseColorTexture = -1;
	int32_t NormalTexture = -1;
	int32_t MetallicRoughnessTexture = -1;
	int32_t OcclusionTexture = -1;
	int32_t EmissiveTexture = -1;

	float MetallicFactor = 1.0f;
	float RoughnessFactor = 1.0f;
	float OcclusionFactor = 1.0f;
	Vec3F EmissiveFactor = { 1.0f, 1.0f, 1.0f };
};
