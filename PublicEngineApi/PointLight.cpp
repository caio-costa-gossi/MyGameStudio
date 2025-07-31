#include "PointLight.h"

PointLight::PointLight()
{
	LightSource::type_ = enums::point;
}

PointLight::PointLight(Vec3F pos, const ColorRgb& color, const float distance) :
	LightSource(std::move(pos), color), distance_(distance)
{
	quadraticFactor_ = distance_ / 5;
	linearFactor_ = (distanceApproxA_ / distance_) - (distanceApproxB_ * quadraticFactor_);

	LightSource::type_ = enums::point;
}

PointLight::PointLight(Vec3F pos, const ColorRgb& color, const float linear, const float quadratic) :
	LightSource(std::move(pos), color), linearFactor_(linear), quadraticFactor_(quadratic)
{
	distance_ = distanceApproxA_ / (linearFactor_ + distanceApproxB_ * quadraticFactor_);

	LightSource::type_ = enums::directional;
}

Err PointLight::SetLightUniforms(const Shader& shader)
{
	shader.SetUniform("pos", pos_.X, pos_.Y, pos_.Z);
	shader.SetUniform("color", color_.R, color_.G, color_.B);
	shader.SetUniform("constant", 1.0f);
	shader.SetUniform("linear", linearFactor_);
	shader.SetUniform("quadratic", quadraticFactor_);

	return error_const::SUCCESS;
}

float PointLight::GetLinear() const
{
	return linearFactor_;
}

float PointLight::GetQuadratic() const
{
	return quadraticFactor_;
}

float PointLight::GetDistance() const
{
	return distance_;
}

Err PointLight::SetLinear(const float linearFactor)
{
	linearFactor_ = linearFactor;
	distance_ = distanceApproxA_ / (linearFactor_ + distanceApproxB_ * quadraticFactor_);

	return error_const::SUCCESS;
}

Err PointLight::SetQuadratic(const float quadraticFactor)
{
	quadraticFactor_ = quadraticFactor;
	distance_ = distanceApproxA_ / (linearFactor_ + distanceApproxB_ * quadraticFactor_);

	return error_const::SUCCESS;
}

Err PointLight::SetDistance(const float distance)
{
	distance_ = distance;
	linearFactor_ = (distanceApproxA_ / distance_) - (distanceApproxB_ * quadraticFactor_);

	return error_const::SUCCESS;
}


float PointLight::distanceApproxA_ = 1.0f;
float PointLight::distanceApproxB_ = 0.5f;