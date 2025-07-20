#pragma once
#include "Enums.h"
#include "Err.h"
#include "Transform.h"

class Camera
{
protected:
	enums::CameraType type_ = enums::perspective;

	float nearPlane_ = 0.0f;
	float farPlane_ = 0.0f;

	Vec3F cameraPos_ = { 0,0,0 };
	Vec3F cameraFront_ = { 0,0,-1 };
	Vec3F cameraUp_ = { 0,1,0 };

	Transform view_;
	Transform projection_;

public:
	Camera() = default;

	Err Move(const Vec3F& newPos);

	[[nodiscard]]
	enums::CameraType GetType() const;

	[[nodiscard]]
	Vec3F GetPos() const;

	[[nodiscard]]
	Vec3F GetTarget() const;

	[[nodiscard]]
	Transform GetView() const;

	[[nodiscard]]
	Transform GetProjection() const;
};
