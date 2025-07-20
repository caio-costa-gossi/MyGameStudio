#include "Camera.h"

#include "NumericUtils.h"

Err Camera::Move(const Vec3F& newPos)
{
	cameraPos_ = newPos;
	Err err = UpdateView();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err Camera::ChangePitch(const float newPitchDegrees)
{
	pitchDegrees_ = newPitchDegrees;
	Err err = UpdateView();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err Camera::ChangeYaw(const float newYawDegrees)
{
	yawDegrees_ = newYawDegrees;
	Err err = UpdateView();
	if (err.Code())
		return err;

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

float Camera::GetPitch() const
{
	return pitchDegrees_;
}

float Camera::GetYaw() const
{
	return yawDegrees_;
}

Vec3F Camera::GetDirection() const
{
	return direction_;
}

Vec3F Camera::GetRight() const
{
	return right_;
}

Vec3F Camera::GetNormal() const
{
	return normal_;
}

Transform Camera::GetView() const
{
	return view_;
}

Transform Camera::GetProjection() const
{
	return projection_;
}

Err Camera::CalculateCameraAxes()
{
	// Direction
	direction_.X = cos(NumericUtils::Radians(Camera::yawDegrees_)) * cos(NumericUtils::Radians(Camera::pitchDegrees_));
	direction_.Y = sin(NumericUtils::Radians(pitchDegrees_));
	direction_.Z = sin(NumericUtils::Radians(yawDegrees_)) * cos(NumericUtils::Radians(pitchDegrees_));

	// Right
	right_ = NumericUtils::Normalize(NumericUtils::CrossProduct(direction_, cameraUp_));

	// Normal
	normal_ = NumericUtils::Normalize(NumericUtils::CrossProduct(right_, direction_));

	return error_const::SUCCESS;
}

Err Camera::UpdateView()
{
	Err err = CalculateCameraAxes();
	if (err.Code())
		return err;

	view_ = glm::lookAt(static_cast<glm::vec3>(Camera::cameraPos_), static_cast<glm::vec3>(Camera::cameraPos_ + direction_), static_cast<glm::vec3>(Camera::cameraUp_));
	return error_const::SUCCESS;
}
