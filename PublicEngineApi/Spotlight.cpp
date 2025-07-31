#include "Spotlight.h"

#include "NumericUtils.h"

Spotlight::Spotlight()
{
	LightSource::type_ = enums::spot;
}

Spotlight::Spotlight(Vec3F pos, const ColorRgb& color, Vec3F direction, const float innerCutoffDegrees, const float outerCutoffDegrees, const float intensity) :
	LightSource(std::move(pos), color), direction_(std::move(direction)), intensity_(intensity)
{
	innerCutoffCos_ = std::cos(NumericUtils::Radians(innerCutoffDegrees));
	outerCutoffCos_ = std::cos(NumericUtils::Radians(outerCutoffDegrees));

	LightSource::type_ = enums::spot;
}

Err Spotlight::SetLightUniforms(const Shader& shader)
{
	shader.SetUniform("pos", pos_.X, pos_.Y, pos_.Z);
	shader.SetUniform("color", color_.R, color_.G, color_.B);
	shader.SetUniform("intensity", intensity_);
	shader.SetUniform("direction", direction_.X, direction_.Y, direction_.Z);
	shader.SetUniform("innerCutoffAngle", innerCutoffCos_);
	shader.SetUniform("outerCutoffAngle", outerCutoffCos_);

	return error_const::SUCCESS;
}

const Vec3F& Spotlight::GetDirection() const
{
	return direction_;
}

float Spotlight::GetIntensity() const
{
	return intensity_;
}

float Spotlight::GetInnerCutoffDegrees() const
{
	return NumericUtils::Degrees(std::acos(innerCutoffCos_));
}

float Spotlight::GetOuterCutoffDegrees() const
{
	return NumericUtils::Degrees(std::acos(outerCutoffCos_));
}

Err Spotlight::SetDirection(const Vec3F& direction)
{
	direction_ = direction;
	return error_const::SUCCESS;
}

Err Spotlight::SetIntensity(const float intensity)
{
	intensity_ = intensity;
	return error_const::SUCCESS;
}

Err Spotlight::SetInnerCutoff(const float innerCutoffDegrees)
{
	innerCutoffCos_ = std::cos(NumericUtils::Radians(innerCutoffDegrees));
	return error_const::SUCCESS;
}

Err Spotlight::SetOuterCutoff(const float outerCutoffDegrees)
{
	outerCutoffCos_ = std::cos(NumericUtils::Radians(outerCutoffDegrees));
	return error_const::SUCCESS;
}
