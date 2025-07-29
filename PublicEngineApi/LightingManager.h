#pragma once
#include <unordered_map>
#include <vector>

#include "Err.h"
#include "LightSource.h"
#include "MVector.h"
#include "Shader.h"

class LightingManager
{
private:
	static float ambientLightStr_;
	static ColorRgb ambientLightClr_;

	static std::unordered_map<uint32_t, LightSource> localLights_;
	static uint32_t lightSrcCounter_;

	static uint32_t bulbAsset_;

public:
	static Err Startup();
	static Err DrawLightSources();
	static Err Shutdown();

	static Err SetLightUniforms(const Shader& shader);

	// Ambient light
	static Err SetAmbientLight(float intensity, const ColorRgb& color);
	static float GetAmbientLightIntensity();
	static ColorRgb GetAmbientLightColor();

	// Local lights
	static Err AddLightSource(const Vec3F& pos, const ColorRgb& color, float intensity, uint32_t& lightSrcId);
	static LightSource* GetLightSource(uint32_t lightSourceId);
};
