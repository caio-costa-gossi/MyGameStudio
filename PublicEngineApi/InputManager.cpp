#include "InputManager.h"
#include <windows.h>
#include "DILayer.h"
#include "GameConsoleManager.h"
#include "SDLInputLayer.h"

Err InputManager::Startup(const HWND hWindow)
{
	/*Err err = DILayer::Startup(hWindow);
	if (err.Code())
		return err;*/

	Err err = SDLInputLayer::Startup(hWindow);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Update()
{
	/*Err err = DILayer::Update();
	if (err.Code())
		return err;*/

	Err err = SDLInputLayer::Update();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Shutdown()
{
	/*Err err = DILayer::Shutdown();
	if (err.Code())
		return err;*/

	Err err = SDLInputLayer::Shutdown();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::SubBeforeInput(const Subscription& sub)
{
	return error_const::SUCCESS;
}