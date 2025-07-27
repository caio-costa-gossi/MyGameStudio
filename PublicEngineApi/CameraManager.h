#pragma once
#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "InputState.h"

using CameraList = std::unordered_map<uint16_t, std::unique_ptr<Camera>>;

class CameraManager
{
private:
	static CameraList cameraList_;
	static uint16_t cameraCounter_;

	static Camera* mainCamera_;
	static std::unique_ptr<Camera> defaultCamera_;

	static bool isInit_;
	static bool isControlEnabled_;

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static bool IsInit();

	static Err CreateOrtho(uint16_t& cameraId, enums::CameraControlScheme controlScheme = enums::free_cam, float leftPlane = 0.0f, float rightPlane = 800.0f, float topPlane = 0.0f, float bottomPlane = 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	static Err CreatePerspective(uint16_t& cameraId, enums::CameraControlScheme controlScheme = enums::free_cam, float width = 800.0f, float height = 600.0f, float fovDegrees = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

	static Err BindMainCamera(uint16_t cameraId);
	static Err UnbindMainCamera();
	static Camera* GetMainCamera();
	static Camera* GetCamera(uint16_t cameraId);
};
