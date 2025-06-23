#pragma once
#include "CallbackData.h"
#include "MVector.h"

class GameObject
{
protected:
	Vec3I64 pos_;

public:
	GameObject() = default;
	void SetPos(const Vec2I64& pos) { pos_.X = pos.X; pos_.Y = pos.Y; pos_.Z = 0; }
	void SetPos(const Vec3I64& pos) { pos_.X = pos.X; pos_.Y = pos.Y; pos_.Z = pos.Z; }
	Vec3I64 GetPos() const { return pos_; }

	virtual ~GameObject() = default;

	virtual void Update(uint64_t delta) = 0;
	virtual void OnGamepadChange(const CallbackData* data) = 0;
	virtual void OnKeyboardChange(const CallbackData* data) = 0;
	virtual void OnMouseChange(const CallbackData* data) = 0;
};