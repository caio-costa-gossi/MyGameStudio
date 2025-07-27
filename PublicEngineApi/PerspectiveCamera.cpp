#include "PerspectiveCamera.h"

#include "NumericUtils.h"

Err PerspectiveCamera::Init(const enums::CameraControlScheme controlScheme, const float width, const float height, const float fovDegrees, const float nearPlane, const float farPlane)
{
	Camera::controlScheme_ = controlScheme;

	width_ = width;
	height_ = height;
	fovRadians_ = NumericUtils::Radians(fovDegrees);

	Camera::nearPlane_ = nearPlane;
	Camera::farPlane_ = farPlane;

	Camera::projection_ = glm::perspective(fovRadians_, width_ / height_, nearPlane_, farPlane_);

	Err err = Camera::UpdateView();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err PerspectiveCamera::ChangeFov(const float newFovDegrees)
{
	fovRadians_ = NumericUtils::Radians(newFovDegrees);
	Camera::projection_ = glm::perspective(fovRadians_, width_ / height_, nearPlane_, farPlane_);

	return error_const::SUCCESS;
}
