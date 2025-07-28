#include "MasterLoopManager.h"
#include "AnimationManager.h"
#include "CameraManager.h"
#include "GameConsoleManager.h"
#include "GameDebuggerChild.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "WindowManager.h"

Err MasterLoopManager::Run()
{
	loopRunning_ = true;

	while (loopRunning_)
	{
		// Update time
		mainGameTimeline_.UpdateLastTime();
		fps_ = mainGameTimeline_.GetUnitMultiplier() / mainGameTimeline_.GetDelta();
		frameStart_ = mainGameTimeline_.GetElapsed();

		// Handle debug info
		if (debug_)
		{
			Err err = GameDebuggerChild::SendInfo();
			if (err.Code())
				GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::debugger);
		}

		// Update game
		UpdateGame();

		// Get time elapsed, control FPS
		frameEnd_ = mainGameTimeline_.GetElapsed();
		const int64_t remainingTime = static_cast<int64_t>(desiredFramePeriod_) - (frameEnd_ - frameStart_);

		if (remainingTime > 0)
			Sleep(static_cast<uint32_t>(remainingTime * 1000) / mainGameTimeline_.GetUnitMultiplier());
	}

	return error_const::SUCCESS;
}

Err MasterLoopManager::Startup(const int argc, char** argv)
{
	// Main game timeline and frame period in timeline units
	mainGameTimeline_ = Timeline(timeline::MICROSECOND);
	desiredFramePeriod_ = mainGameTimeline_.GetUnitMultiplier() / desiredFps_;

	if (argc < 5)
		return error_const::GAME_INIT_INVALID_PARAMS;
	if (argc > 5)
		debug_ = true;

	Err err = GameConsoleManager::Startup(argv[static_cast<uint8_t>(enums::GameExeArguments::verbosity_level)], argv[static_cast<uint8_t>(enums::GameExeArguments::disable_log_channels)]);
	if (err.Code())
	{
		std::cout << error_const::CONSOLE_MANAGER_STARTUP_FAIL.Message();
		return err;
	}

	// Subsystem startup

	// Debugging information communication pipe
	if (debug_)
	{
		err = GameDebuggerChild::InitDebuggerPipe(argc, argv);
		if (err.Code())
			return err;
	}

	// Window Manager
	err = WindowManager::Startup();
	if (err.Code())
		return err;

	// Input Manager
	err = InputManager::Startup(WindowManager::GetWindowHandle(), argv[static_cast<uint8_t>(enums::GameExeArguments::deadzone)], argv[static_cast<uint8_t>(enums::GameExeArguments::use_sdl)]);
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	// Game Object Manager
	GameConsoleManager::PrintInfo("Starting GameObjectManager...", enums::ConsoleMessageSender::loop);
	err = GameObjectManager::Get().Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::object);

	// Camera Manager
	GameConsoleManager::PrintInfo("Starting CameraManager...", enums::ConsoleMessageSender::loop);
	err = CameraManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::camera);
	
	// Render Manager
	GameConsoleManager::PrintInfo("Starting RenderManager...", enums::ConsoleMessageSender::loop);
	err = RenderManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);


	// Systems not implemented yet
	GameConsoleManager::PrintInfo("Starting PhysicsManager...", enums::ConsoleMessageSender::loop);
	err = PhysicsManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::physics);

	GameConsoleManager::PrintInfo("Starting AnimationManager...", enums::ConsoleMessageSender::loop);
	err = AnimationManager::Startup();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::animation);

	// Start time
	mainGameTimeline_.Start();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Shutdown()
{
	// Stop time
	mainGameTimeline_.Pause();

	// Shutdown subsystems

	Err err = WindowManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

	err = InputManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	if (debug_)
	{
		err = GameDebuggerChild::Shutdown();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::debugger);
	}

	err = GameObjectManager::Get().Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::object);

	err = PhysicsManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::physics);

	err = AnimationManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::animation);

	err = RenderManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

	return error_const::SUCCESS;
}

Err MasterLoopManager::UpdateGame()
{
	// Update subsystems
	Err err = WindowManager::Update();
	if (err.Code() == error_const::EXIT_REQUEST_CODE)
		loopRunning_ = false;

	InputManager::Update();
	GameObjectManager::Get().Update(mainGameTimeline_.GetDelta());
	CameraManager::Update();
	RenderManager::Update();

	// Not implemented yet
	PhysicsManager::Update();
	AnimationManager::Update();

	return error_const::SUCCESS;
}

Err MasterLoopManager::Stop()
{
	loopRunning_ = false;
	return error_const::SUCCESS;
}

bool MasterLoopManager::loopRunning_;
bool MasterLoopManager::debug_ = false;

Timeline MasterLoopManager::mainGameTimeline_;

int64_t MasterLoopManager::frameStart_ = 0;
int64_t MasterLoopManager::frameEnd_ = 0;
uint64_t MasterLoopManager::desiredFps_ = 120;
uint64_t MasterLoopManager::desiredFramePeriod_;
uint64_t MasterLoopManager::fps_ = 0;