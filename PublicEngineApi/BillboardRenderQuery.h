#pragma once
#include <cstdint>

#include "BillboardData.h"
#include "CameraManager.h"

static float Pow2(const float x) { return x * x; }

struct BillboardRenderQuery
{
	BillboardData BillboardData;
	uint32_t BillboardVao;

	// Compare to see what query is nearest to active camera
	bool operator< (const BillboardRenderQuery& other) const
	{
		assert(CameraManager::IsInit());

		const Vec3F camPos = CameraManager::GetMainCamera()->GetPos();
		const Vec3F thisPos = BillboardData.WorldPos;
		const Vec3F otherPos = other.BillboardData.WorldPos;

		const float thisDistance = Pow2(camPos.X - thisPos.X) + Pow2(camPos.Y - thisPos.Y) + Pow2(camPos.Z - thisPos.Z);
		const float otherDistance = Pow2(camPos.X - otherPos.X) + Pow2(camPos.Y - otherPos.Y) + Pow2(camPos.Z - otherPos.Z);

		return thisDistance < otherDistance;
	}
};
