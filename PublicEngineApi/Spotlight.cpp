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

Err Spotlight::SetLightUniforms(const Shader& shader, uint32_t directionalCount, uint32_t pointCount, uint32_t spotCount)
{
	const std::string uniform = "spotlights[" + std::to_string(spotCount) + "]";

	shader.SetUniform((uniform + ".pos").c_str(), pos_.X, pos_.Y, pos_.Z);
	shader.SetUniform((uniform + ".color").c_str(), color_.R, color_.G, color_.B);
	shader.SetUniform((uniform + ".intensity").c_str(), intensity_);
	shader.SetUniform((uniform + ".direction").c_str(), direction_.X, direction_.Y, direction_.Z);
	shader.SetUniform((uniform + ".innerCutoffAngle").c_str(), innerCutoffCos_);
	shader.SetUniform((uniform + ".outerCutoffAngle").c_str(), outerCutoffCos_);

	return error_const::SUCCESS;
}

Vec3F& Spotlight::GetDirection()
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
