#include "CameraManager.h"

#include "CameraFactory.h"

Err CameraManager::Startup()
{
	defaultCamera_ = CameraFactory::CreatePerspective();
	isInit_ = true;

	return error_const::SUCCESS;
}

Err CameraManager::Shutdown()
{
	return error_const::SUCCESS;
}

bool CameraManager::IsInit()
{
	return isInit_;
}

Err CameraManager::CreateOrtho(uint16_t& cameraId, const float leftPlane, const float rightPlane, const float topPlane, const float bottomPlane, const float nearPlane, const float farPlane)
{
	std::unique_ptr<Camera> newCamera = CameraFactory::CreateOrtho(leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane);
	cameraList_[++cameraCounter_] = std::move(newCamera);
	cameraId = cameraCounter_;

	return error_const::SUCCESS;
}

Err CameraManager::CreatePerspective(uint16_t& cameraId, const float width, const float height, const float fovDegrees, const float nearPlane, const float farPlane)
{
	std::unique_ptr<Camera> newCamera = CameraFactory::CreatePerspective(width, height, fovDegrees, nearPlane, farPlane);
	cameraList_[++cameraCounter_] = std::move(newCamera);
	cameraId = cameraCounter_;

	return error_const::SUCCESS;
}

Err CameraManager::BindMainCamera(const uint16_t cameraId)
{
	if (cameraList_.find(cameraId) == cameraList_.end())
		return error_const::RENDER_CAMERA_NOT_FOUND;

	mainCamera_ = cameraList_.at(cameraId).get();

	return error_const::SUCCESS;
}

Camera* CameraManager::GetMainCamera()
{
	if (mainCamera_ == nullptr)
		return defaultCamera_.get();

	return mainCamera_;
}

Camera* CameraManager::GetCamera(const uint16_t cameraId)
{
	if (cameraList_.find(cameraId) == cameraList_.end())
		return nullptr;

	return cameraList_.at(cameraId).get();
}


CameraList CameraManager::cameraList_ = CameraList();
uint16_t CameraManager::cameraCounter_ = 0;

Camera* CameraManager::mainCamera_ = nullptr;
std::unique_ptr<Camera> CameraManager::defaultCamera_;

bool CameraManager::isInit_ = false;