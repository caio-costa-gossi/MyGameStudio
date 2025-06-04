#pragma once
#include "MVector.h"

class GameObject
{
protected:
	uint64_t xPos_ = 0;
	uint64_t yPos_ = 0;
	uint64_t zPos_ = 0;

public:
	GameObject() = default;
	void SetPos(const Vec2U64& pos) { xPos_ = pos.X; yPos_ = pos.Y; }
	void SetPos(const Vec3U64& pos) { xPos_ = pos.X; yPos_ = pos.Y; zPos_ = pos.Z; }
	Vec3U64 GetPos() { return { xPos_, yPos_, zPos_ }; }

	virtual ~GameObject() = default;

	virtual void Update(uint64_t delta) = 0;
};