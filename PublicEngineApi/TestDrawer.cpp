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

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	Vertex* vertices = new Vertex[4];

	vertices[0] = { {0.5f,  0.5f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f } };
	vertices[1] = { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };
	vertices[2] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } };
	vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };

	uint32_t* indices = new uint32_t[6];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	testMesh_ = { 1, vertices, 4, indices, 6, 46 };

	running_ = true;

	while (running_)
	{
		rotation_ += 0.1f;
		scale_ += {0.0001f, 0.0001f, 0};

		Err err = WindowManager::Update();
		if (err.Code() == error_const::EXIT_REQUEST_CODE)
			running_ = false;
		else if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

		err = InputManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

		RenderRequest request = { &testMesh_, position_, rotation_, {0,0,1}, scale_};
		err = RenderManager::RequestRender(request);
		if (err.Code())
			return err;

		err = RenderManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::render);
	}

	delete[] indices;
	delete[] vertices;

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
Mesh TestDrawer::testMesh_;

float TestDrawer::rotation_ = 0;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};