#pragma once
#include "Enums.h"
#include "Err.h"
#include "MVector.h"

class CameraInstance
{
private:
	uint16_t globalCameraId_ = 0;

public:
	CameraInstance(enums::CameraType cameraType = enums::perspective, enums::CameraControlScheme controlScheme = enums::free_cam, float nearPlane = 0.1f, float farPlane = 100.0f, float perspectiveWidth = 800.0f, float perspectiveHeight = 600.0f, float perspectiveFovDegrees = 45.0f, float orthoLeftPlane = 0.0f, float orthoRightPlane = 800.0f, float orthoTopPlane = 0.0f, float orthoBottomPlane = 600.0f);

	Err Use() const ;

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
};
