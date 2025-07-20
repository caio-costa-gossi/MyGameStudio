#pragma once
#include <memory>
#include <unordered_map>

#include "Camera.h"

using CameraList = std::unordered_map<uint16_t, std::unique_ptr<Camera>>;

class CameraManager
{
private:
	static CameraList cameraList_;
	static uint16_t cameraCounter_;

	static Camera* mainCamera_;
	static std::unique_ptr<Camera> defaultCamera_;

public:
	static Err Startup();
	static Err Shutdown();

	static Err CreateOrtho(uint16_t& cameraId, float leftPlane = 0.0f, float rightPlane = 800.0f, float topPlane = 0.0f, float bottomPlane = 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	static Err CreatePerspective(uint16_t& cameraId, float width = 800.0f, float height = 600.0f, float fovDegrees = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

	static Err BindMainCamera(uint16_t cameraId);
	static Camera* GetMainCamera();

};
