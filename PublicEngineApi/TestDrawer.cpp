#include "TestDrawer.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "NumericUtils.h"
#include "WindowManager.h"
#include "RenderManager.h"
#include "Transform.h"


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

	time_.Start();

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	Vertex* vertices = new Vertex[36];

	/*vertices[0] = { {0.5f,  0.5f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f}, { 1.0f, 1.0f } };
	vertices[1] = { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };
	vertices[2] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } };
	vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };*/

	vertices[0] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	vertices[1] = { {0.5f, -0.5f, -0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[2] = { {0.5f,  0.5f, -0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[3] = { {0.5f,  0.5f, -0.5f},  {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[4] = { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[5] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };

	vertices[6] = { {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	vertices[7] = { {0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[8] = { {0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[9] = { {0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[10] = { {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[11] = { {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };

	vertices[12] = { {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[13] = { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[14] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[15] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[16] = { {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	vertices[17] = { {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };

	vertices[18] = { {0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[19] = { {0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[20] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[21] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[22] = { {0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	vertices[23] = { {0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };

	vertices[24] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[25] = { {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} };
	vertices[26] = { {0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[27] = { {0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} };
	vertices[28] = { {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	vertices[29] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };

	vertices[30] = { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} };
	vertices[31] = { {0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, };
	vertices[32] = { {0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f},};
	vertices[33] = {{0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f},};
	vertices[34] = {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
	vertices[35] = {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};

	uint32_t* indices = new uint32_t[6];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	testMesh_ = { 1, vertices, 36, indices, 6, 47 };

	Vec3F camPos = { 0.0f, 0.0f, 3.0f };
	Vec3F camTarget = { 0.0f, 0.0f, 0.0f };
	Vec3F camDirection = NumericUtils::NormalizeVec3(camPos - camTarget);

	running_ = true;

	while (running_)
	{
		Vec3F worldPos[5] = { {0.0f, 0.0f, 0.0f}, {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f} };

		
		Transform view({ 0,sin(static_cast<float>(time_.GetElapsed()) / 3000),-3.0f}, 0, {0,0,1}, {1,1,1});
		Transform projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		Err err = WindowManager::Update();
		if (err.Code() == error_const::EXIT_REQUEST_CODE)
			running_ = false;
		else if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

		err = InputManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

		for (int i = 0; i < 5; ++i)
		{
			Transform model(worldPos[i], static_cast<float>(time_.GetElapsed()) / 50, {0.5f,1,0}, {1,1,1});

			RenderRequest request = { &testMesh_, model, view, projection };
			err = RenderManager::RequestRender(request);
			if (err.Code())
				return err;
		}

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

float TestDrawer::rotation_ = -55.0f;
Vec3F TestDrawer::position_ = {0,0,0};
Vec3F TestDrawer::scale_ = {1,1,1};

Timeline TestDrawer::time_(timeline::MILLISECOND);