#include "OrthoCamera.h"

Err OrthoCamera::Init(const enums::CameraControlScheme controlScheme, const float leftPlane, const float rightPlane, const float topPlane, const float bottomPlane, const float nearPlane, const float farPlane)
{
	Camera::controlScheme_ = controlScheme;

	leftPlane_ = leftPlane;
	rightPlane_ = rightPlane;
	topPlane_ = topPlane;
	bottomPlane_ = bottomPlane;

	Camera::farPlane_ = farPlane;
	Camera::nearPlane_ = nearPlane;

	Camera::projection_ = glm::ortho(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

	Err err = Camera::UpdateView();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}
