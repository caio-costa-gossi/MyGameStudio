#include "RenderManager.h"

#include <glad/glad.h>

#include "AssetRuntimeManager.h"
#include "BillboardDrawer.h"
#include "Drawer.h"
#include "Enums.h"
#include "GameConsoleManager.h"
#include "ModelDrawer.h"
#include "WindowManager.h"

Err RenderManager::Startup()
{
	renderTime_.Start();

	gameWindow_ = WindowManager::GetSdlWindow();

	// Init Renderer
	Err err = InitRenderer();
	if (err.Code())
		return err;

	// Init Drawer
	err = Drawer::Init();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::Update()
{
	Err err = Draw();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::Shutdown()
{
	GameConsoleManager::PrintInfo("Shutting down OpenGL context...", enums::ConsoleMessageSender::render);
	SDL_GL_DestroyContext(glContext_);

	return error_const::SUCCESS;
}

Err RenderManager::InitRenderer()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Init OpenGL Context
	GameConsoleManager::PrintInfo("Initializing OpenGL context...", enums::ConsoleMessageSender::render);
	glContext_ = SDL_GL_CreateContext(gameWindow_);
	if (!glContext_)
		return Err("SDL_GL_CreateContext Error: " + std::string(SDL_GetError()), error_const::SDL_RENDER_ERROR_CODE);

	// Load OpenGL function pointers via GLAD
	GameConsoleManager::PrintInfo("Initializing OpenGL SDL Loader...", enums::ConsoleMessageSender::render);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		SDL_GL_DestroyContext(glContext_);
		return Err("Failed to initialize GLAD", error_const::SDL_RENDER_ERROR_CODE);
	}

	// Set viewport
	viewport_ = { 0, 0, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight() };

	// Enable Z-Testing
	glEnable(GL_DEPTH_TEST);

	GameConsoleManager::PrintInfo("Renderer: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
	GameConsoleManager::PrintInfo("Vendor: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
	GameConsoleManager::PrintInfo("Version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

	return error_const::SUCCESS;
}

Err RenderManager::RequestRender(const RenderRequest& request)
{
	Err err = ModelDrawer::RequestDraw(request);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::RequestBillboardRender(const BillboardRenderRequest& request)
{
	Err err = BillboardDrawer::RequestDraw(request);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::Draw()
{
	glViewport(viewport_.X, viewport_.Y, viewport_.Width, viewport_.Height);
	Drawer::Draw();
	SDL_GL_SwapWindow(gameWindow_);

	return error_const::SUCCESS;
}

void RenderManager::ResizeViewport(const int32_t w, const int32_t h)
{
	viewport_ = { 0, 0, w, h };
}


SDL_Window* RenderManager::gameWindow_ = nullptr;
SDL_GLContext RenderManager::glContext_;
Viewport RenderManager::viewport_;

Timeline RenderManager::renderTime_ = Timeline(timeline::MILLISECOND);