#include "CameraInstance.h"

#include "CameraManager.h"

CameraInstance::CameraInstance(const enums::CameraType cameraType, const float nearPlane, const float farPlane, const float perspectiveWidth, const float perspectiveHeight, const float perspectiveFovDegrees, const float orthoLeftPlane, const float orthoRightPlane, const float orthoTopPlane, const float orthoBottomPlane)
{
	assert(CameraManager::IsInit());

	if (cameraType == enums::orthogonal)
		CameraManager::CreateOrtho(globalCameraId_, orthoLeftPlane, orthoRightPlane, orthoTopPlane, orthoBottomPlane, nearPlane, farPlane);
	else
		CameraManager::CreatePerspective(globalCameraId_, perspectiveWidth, perspectiveHeight, perspectiveFovDegrees, nearPlane, farPlane);
}

Err CameraInstance::Use() const
{
	return CameraManager::BindMainCamera(globalCameraId_);
}

Err CameraInstance::Move(const Vec3F& newPos)
{
	return CameraManager::GetCamera(globalCameraId_)->Move(newPos);
}

Err CameraInstance::ChangePitch(const float newPitchDegrees)
{
	return CameraManager::GetCamera(globalCameraId_)->ChangePitch(newPitchDegrees);
}

Err CameraInstance::ChangeYaw(const float newYawDegrees)
{
	return CameraManager::GetCamera(globalCameraId_)->ChangeYaw(newYawDegrees);
}

enums::CameraType CameraInstance::GetType() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetType();
}

Vec3F CameraInstance::GetPos() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetPos();
}

float CameraInstance::GetPitch() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetPitch();
}

float CameraInstance::GetYaw() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetYaw();
}

Vec3F CameraInstance::GetDirection() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetDirection();
}

Vec3F CameraInstance::GetRight() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetRight();
}

Vec3F CameraInstance::GetNormal() const
{
	return CameraManager::GetCamera(globalCameraId_)->GetNormal();
}
