#include "InputManager.h"
#include <windows.h>
#include "DILayer.h"
#include "GameConsoleManager.h"
#include "SDLInputLayer.h"

Err InputManager::Startup(const HWND hWindow, const bool usingSdl)
{
	// Define input layer
	if (usingSdl)
		inputLayer_ = new SDLInputLayer();
	else
		inputLayer_ = new DILayer();

	// Startup instance
	Err err = inputLayer_->Startup(hWindow);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Update()
{
	Err err = inputLayer_->Update();
	if (err.Code())
		return err;

	inputState_ = inputLayer_->GetInputStates();

	return error_const::SUCCESS;
}

Err InputManager::Shutdown()
{
	Err err = inputLayer_->Shutdown();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::SubBeforeInput(const Subscription& sub)
{
	return error_const::SUCCESS;
}


InputLayer* InputManager::inputLayer_ = nullptr;
InputState InputManager::inputState_;