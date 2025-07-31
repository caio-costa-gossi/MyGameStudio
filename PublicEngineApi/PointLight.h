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

	Err SetLightUniforms(Shader& shader, uint32_t directionalCount, uint32_t pointCount, uint32_t spotCount) override;

	float GetLinear() const;
	float GetQuadratic() const;
	float GetDistance() const;

	Err SetLinear(float linearFactor);
	Err SetQuadratic(float quadraticFactor);
	Err SetDistance(float distance);
};