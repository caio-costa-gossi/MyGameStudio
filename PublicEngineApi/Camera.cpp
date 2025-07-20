#include "Camera.h"

Err Camera::Move(const Vec3F& newPos)
{
	cameraPos_ = newPos;
	view_ = glm::lookAt(static_cast<glm::vec3>(Camera::cameraPos_), static_cast<glm::vec3>(Camera::cameraPos_ + Camera::cameraFront_), static_cast<glm::vec3>(Camera::cameraUp_));

	return error_const::SUCCESS;
}

enums::CameraType Camera::GetType() const
{
	return type_;
}

Vec3F Camera::GetPos() const
{
	return cameraPos_;
}

Vec3F Camera::GetTarget() const
{
	return cameraPos_ + cameraFront_;
}

Transform Camera::GetView() const
{
	return view_;
}

Transform Camera::GetProjection() const
{
	return projection_;
}
