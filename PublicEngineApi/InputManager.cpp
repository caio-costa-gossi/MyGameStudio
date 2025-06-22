#include "InputManager.h"
#include <windows.h>
#include "DILayer.h"
#include "GameConsoleManager.h"
#include "SDLInputLayer.h"

Err InputManager::Startup(const HWND hWindow, const int32_t deadzone, const bool usingSdl)
{
	// Define input layer
	if (usingSdl)
		inputLayer_ = new SDLInputLayer();
	else
		inputLayer_ = new DILayer();

	// Startup instance
	Err err = inputLayer_->Startup(hWindow, deadzone);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Update(const SDL_Event* eventList, const uint32_t numEvent)
{
	Err err = inputLayer_->Update(eventList, numEvent);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::Shutdown()
{
	Err err = inputLayer_->Shutdown();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err InputManager::SubForInputEvent(const Subscription& sub)
{
	inputLayer_->Subscribe(sub);
	return error_const::SUCCESS;
}

const InputState& InputManager::GetInputState()
{
	return inputLayer_->GetInputState();
}


InputLayer* InputManager::inputLayer_ = nullptr;