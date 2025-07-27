#include "Camera.h"

#include "InputManager.h"
#include "NumericUtils.h"
#include "WindowManager.h"

#undef min

Err Camera::Update()
{
	if (controlScheme_ == enums::free_cam)
	{
		Err err = UpdateFreeCam();
		if (err.Code())
			return err;
	}
	else
	{
		Err err = UpdateBoundCam();
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

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

enums::CameraControlScheme Camera::GetControlScheme() const
{
	return controlScheme_;
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

Err Camera::UpdateFreeCam()
{
	if (!WindowManager::IsMouseRelative())
		return error_const::SUCCESS;

	const InputState state = InputManager::GetInputState();

	// Pos
	if (state.KeyboardState.PhysicalKeyState[keyboard_key_w])
		Move(GetPos() + GetDirection() * 0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_s])
		Move(GetPos() + GetDirection() * -0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_a])
		Move(GetPos() + GetRight() * -0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_d])
		Move(GetPos() + GetRight() * 0.01f);

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_space])
		Move(GetPos() + Vec3F(0, 0.01f, 0));

	if (state.KeyboardState.PhysicalKeyState[keyboard_key_lshift])
		Move(GetPos() + Vec3F(0, -0.01f, 0));

	// Direction
	ChangeYaw(GetYaw() + state.MouseState.XVel / 5.0f);

	float newPitch = GetPitch() - state.MouseState.YVel / 5.0f;
	newPitch = std::min(newPitch, 89.0f);
	newPitch = std::max(newPitch, -89.0f);
	ChangePitch(newPitch);

	return error_const::SUCCESS;
}

Err Camera::UpdateBoundCam() const
{
	return error_const::SUCCESS;
}