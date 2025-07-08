#include "InputManager.h"
#include <windows.h>
#include "DILayer.h"
#include "GameConsoleManager.h"
#include "SDLInputLayer.h"
#include "WindowManager.h"

Err InputManager::Startup(const HWND hWindow, const int32_t deadzone, const bool usingSdl)
{
	usingSdl_ = usingSdl;

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

Err InputManager::Update()
{
	uint32_t numEvent = 0;
	const SDL_Event* eventList = nullptr;

	if (usingSdl_)
		eventList = WindowManager::GetEventList(numEvent);

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
bool InputManager::usingSdl_ = false;