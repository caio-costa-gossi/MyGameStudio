#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	LightSource::type_ = enums::directional;
}

DirectionalLight::DirectionalLight(Vec3F pos, const ColorRgb& color, Vec3F direction, const float intensity) :
	LightSource(std::move(pos), color), direction_(std::move(direction)), intensity_(intensity)
{
	LightSource::type_ = enums::directional;
}

Err DirectionalLight::SetLightUniforms(const Shader& shader)
{
	shader.SetUniform("color", color_.R, color_.G, color_.B);
	shader.SetUniform("direction", direction_.X, direction_.Y, direction_.Z);
	shader.SetUniform("intensity", intensity_);

	return error_const::SUCCESS;
}

const Vec3F& DirectionalLight::GetDirection() const
{
	return direction_;
}

float DirectionalLight::GetIntensity() const
{
	return intensity_;
}

Err DirectionalLight::SetDirection(const Vec3F& direction)
{
	direction_ = direction;
	return error_const::SUCCESS;
}

Err DirectionalLight::SetIntensity(const float intensity)
{
	intensity_ = intensity;
	return error_const::SUCCESS;
}
