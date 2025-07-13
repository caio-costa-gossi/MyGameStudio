#include "TestDrawer.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "RenderManager.h"

Err TestDrawer::Startup()
{
	Err err = GameConsoleManager::Startup("0", "0");
	if (err.Code())
		return err;

	err = WindowManager::Startup();
	if (err.Code())
		return err;

	err = InputManager::Startup(WindowManager::GetWindowHandle(), "2500", "1");
	if (err.Code())
		return err;

	err = RenderManager::Startup();
	if (err.Code())
		return err;

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	float vertices2[] = {
	 0.75f,  0.75f, 0.5f,  // top right
	 0.75f, -0.25f, 0.25f,  // bottom right
	-0.25f, -0.25f, 1.0f,  // bottom left
	-0.25f,  0.75f, 0.0f   // top left 
	};

	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	uint32_t indices2[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	const Mesh mesh = { vertices, 4, indices, 6 };
	//const Mesh mesh2 = { vertices2, 4, indices2, 6 };

	err = RenderManager::AddObject(mesh);
	//err = RenderManager::AddObject(mesh2);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	running_ = true;

	while (running_)
	{
		Err err = WindowManager::Update();
		if (err.Code() == error_const::EXIT_REQUEST_CODE)
			running_ = false;
		else if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

		err = InputManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

		err = RenderManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);
	}

	return error_const::SUCCESS;
}

Err TestDrawer::Shutdown()
{
	Err err = WindowManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	err = InputManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

	err = RenderManager::Shutdown();
	if (err.Code())
		GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);

	return error_const::SUCCESS;
}


bool TestDrawer::running_ = false;
