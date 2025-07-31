#include "PointLight.h"

PointLight::PointLight()
{
	LightSource::type_ = enums::point;
}

PointLight::PointLight(Vec3F pos, const ColorRgb& color, const float distance) :
	LightSource(std::move(pos), color)
{
	distance_ = std::min(distance, 30.0f);

	linearFactor_ = 1.0f / pow(1.7f, distance_);
	quadraticFactor_ = 1.0f / (distance_ * distance_ * distance_) * (linearFactor_ * distance_);

	LightSource::type_ = enums::point;
}

PointLight::PointLight(Vec3F pos, const ColorRgb& color, const float linear, const float quadratic) :
	LightSource(std::move(pos), color), linearFactor_(linear), quadraticFactor_(quadratic)
{
	distance_ = distanceApproxA_ / (linearFactor_ + distanceApproxB_ * quadraticFactor_);

	LightSource::type_ = enums::directional;
}

Err PointLight::SetLightUniforms(const Shader& shader, const uint32_t directionalCount, const uint32_t pointCount, const uint32_t spotCount)
{
	const std::string uniform = "pointLights[" + std::to_string(pointCount) + "]";

	shader.SetUniform((uniform + ".pos").c_str(), pos_.X, pos_.Y, pos_.Z);
	shader.SetUniform((uniform + ".color").c_str(), color_.R, color_.G, color_.B);
	shader.SetUniform((uniform + ".constant").c_str(), 1.0f);
	shader.SetUniform((uniform + ".linear").c_str(), linearFactor_);
	shader.SetUniform((uniform + ".quadratic").c_str(), quadraticFactor_);

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
	distance_ = std::min(distance, 30.0f);

	linearFactor_ = 1.0f / pow(1.7f,distance_);
	quadraticFactor_ = 1.0f / (distance_ * distance_ * distance_) * (linearFactor_ * distance_);

	//quadraticFactor_ = std::max(distance_ / pow(1.4f,distance_), 0.002f);
	//linearFactor_ = (distanceApproxA_ / distance_) - (distanceApproxB_ * quadraticFactor_);

	return error_const::SUCCESS;
}


float PointLight::distanceApproxA_ = 1.0f;
float PointLight::distanceApproxB_ = 0.5f;