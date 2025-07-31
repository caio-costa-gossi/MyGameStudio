#include "LightingManager.h"

#include "AssetRuntimeManager.h"
#include "DirectionalLight.h"
#include "GameConsoleManager.h"
#include "PointLight.h"
#include "RenderManager.h"
#include "Spotlight.h"

Err LightingManager::Startup()
{
	// Load internal engine assets for the light sources
	AssetRuntimeManager::LoadImg("internal_engine_assets/directional.png", directionalAsset_);
	if (directionalAsset_ == 0)
		GameConsoleManager::PrintError("Icon asset for directional light source not loaded.");

	AssetRuntimeManager::LoadImg("internal_engine_assets/point.png", pointAsset_);
	if (pointAsset_ == 0)
		GameConsoleManager::PrintError("Icon asset for point light source not loaded.");

	AssetRuntimeManager::LoadImg("internal_engine_assets/spotlight.png", spotlightAsset_);
	if (spotlightAsset_ == 0)
		GameConsoleManager::PrintError("Icon asset for spotlight source not loaded.");

	return error_const::SUCCESS;
}

Err LightingManager::DrawLightSources()
{
	for (const std::pair<const uint32_t, std::unique_ptr<LightSource>>& source : lights_)
	{
		uint32_t iconId;
		Err err = GetSourceAttributes(source.second->GetType(), iconId);
		if (err.Code())
			return err;

		BillboardRenderRequest bulbRenderRequest = { {source.second->GetPos(), {1.0f,1.0f}, iconId}};
		err = RenderManager::RequestBillboardRender(bulbRenderRequest);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err LightingManager::Shutdown()
{
	return error_const::SUCCESS;
}

Err LightingManager::SetLightUniforms(const Shader& shader)
{
	shader.Use();

	shader.SetUniform("ambientColor", ambientLightColor_.R, ambientLightColor_.G, ambientLightColor_.B);
	shader.SetUniform("ambientFactor", ambientLightIntensity_);

	const Vec3F& viewPos = CameraManager::GetMainCamera()->GetPos();
	shader.SetUniform("viewPos", viewPos.X, viewPos.Y, viewPos.Z);

	for (const std::pair<const uint32_t, std::unique_ptr<LightSource>>& source : lights_)
	{
		Err err = source.second->SetLightUniforms(shader);
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);
	}

	return error_const::SUCCESS;
}

Err LightingManager::SetAmbientLight(const float intensity, const ColorRgb& color)
{
	ambientLightIntensity_ = intensity;
	ambientLightColor_ = color;

	return error_const::SUCCESS;
}

float LightingManager::GetAmbientLightIntensity()
{
	return ambientLightIntensity_;
}

ColorRgb LightingManager::GetAmbientLightColor()
{
	return ambientLightColor_;
}

Err LightingManager::AddDirectionalLight(const Vec3F& pos, const ColorRgb& color, const Vec3F& direction, const float intensity, uint32_t& lightSourceId)
{
	lights_[lightSrcCounter_] = std::make_unique<DirectionalLight>(pos, color, direction, intensity);
	lightSourceId = lightSrcCounter_++;

	return error_const::SUCCESS;
}

Err LightingManager::AddPointLight(const Vec3F& pos, const ColorRgb& color, const float distance, uint32_t& lightSourceId)
{
	lights_[lightSrcCounter_] = std::make_unique<PointLight>(pos, color, distance);
	lightSourceId = lightSrcCounter_++;

	return error_const::SUCCESS;
}

Err LightingManager::AddSpotlight(const Vec3F& pos, const ColorRgb& color, const Vec3F& direction, const float innerCutoffDegrees, const float outerCutoffDegrees, const float intensity, uint32_t& lightSourceId)
{
	lights_[lightSrcCounter_] = std::make_unique<Spotlight>(pos, color, direction, innerCutoffDegrees, outerCutoffDegrees, intensity);
	lightSourceId = lightSrcCounter_++;

	return error_const::SUCCESS;
}

LightSource* LightingManager::GetLightSource(const uint32_t lightSourceId)
{
	if (lights_.find(lightSourceId) == lights_.end())
		return nullptr;

	return lights_[lightSourceId].get();
}

Err LightingManager::GetSourceAttributes(const enums::LightType type, uint32_t& iconId)
{
	switch (type)
	{
	case enums::directional:
		iconId = directionalAsset_;
		break;
	case enums::point:
		iconId = pointAsset_;
		break;
	case enums::spot:
		iconId = spotlightAsset_;
		break;
	default:
		iconId = pointAsset_;
	}

	return error_const::SUCCESS;
}


float LightingManager::ambientLightIntensity_ = 0.4f;
ColorRgb LightingManager::ambientLightColor_ = { 1.0f,1.0f,1.0f };

std::unordered_map<uint32_t, std::unique_ptr<LightSource>> LightingManager::lights_;
uint32_t LightingManager::lightSrcCounter_ = 0;

uint32_t LightingManager::directionalAsset_ = 0;
uint32_t LightingManager::pointAsset_ = 0;
uint32_t LightingManager::spotlightAsset_ = 0;