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

Err DirectionalLight::SetLightUniforms(const Shader& shader, const uint32_t directionalCount, const uint32_t pointCount, const uint32_t spotCount)
{
	const std::string uniform = "dirLights[" + std::to_string(directionalCount) + "]";
	std::string complete = uniform + ".color";
	std::string complete2 = uniform + ".direction";

	shader.SetUniform(complete.c_str(), color_.R, color_.G, color_.B);
	shader.SetUniform(complete2.c_str(), direction_.X, direction_.Y, direction_.Z);
	shader.SetUniform((uniform + ".intensity").c_str(), intensity_);

	return error_const::SUCCESS;
}

Vec3F& DirectionalLight::GetDirection()
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
