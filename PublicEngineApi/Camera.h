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
	Vec3F cameraUp_ = { 0,1,0 };
	float pitchDegrees_ = 0;
	float yawDegrees_ = 0;

	Vec3F direction_ = { 0,0,0 };
	Vec3F right_ = { 0,0,0 };
	Vec3F normal_ = { 0,0,0 };

	Transform view_;
	Transform projection_;

	Err CalculateCameraAxes();
	Err UpdateView();

public:
	Camera() = default;

	Err Move(const Vec3F& newPos);
	Err ChangePitch(float newPitchDegrees);
	Err ChangeYaw(float newYawDegrees);

	[[nodiscard]]
	enums::CameraType GetType() const;

	[[nodiscard]]
	Vec3F GetPos() const;

	[[nodiscard]]
	float GetPitch() const;

	[[nodiscard]]
	float GetYaw() const;

	[[nodiscard]]
	Vec3F GetDirection() const;

	[[nodiscard]]
	Vec3F GetRight() const;

	[[nodiscard]]
	Vec3F GetNormal() const;

	[[nodiscard]]
	Transform GetView() const;

	[[nodiscard]]
	Transform GetProjection() const;
};
