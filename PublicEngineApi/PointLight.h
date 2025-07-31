#pragma once
#include "LightSource.h"

class PointLight : public LightSource
{
private:
	float linearFactor_ = 1.0f;
	float quadraticFactor_ = 1.0f;
	float distance_ = 0.66f;

	static float distanceApproxA_;
	static float distanceApproxB_;

public:
	PointLight();
	PointLight(Vec3F pos, const ColorRgb& color, float linear, float quadratic);
	PointLight(Vec3F pos, const ColorRgb& color, float distance);

	Err SetLightUniforms(const Shader& shader) override;

	float GetLinear() const;
	float GetQuadratic() const;
	float GetDistance() const;

	Err SetLinear(float linearFactor);
	Err SetQuadratic(float quadraticFactor);
	Err SetDistance(float distance);
};