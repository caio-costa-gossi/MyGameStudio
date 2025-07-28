#include "RenderManager.h"

#include <glad/glad.h>

#include "AssetRuntimeManager.h"
#include "CameraManager.h"
#include "Drawer.h"
#include "Enums.h"
#include "GameConsoleManager.h"
#include "MeshInstance.h"
#include "RenderQuery.h"
#include "Texture.h"
#include "Transform.h"
#include "VaoFactory.h"
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

	return error_const::SUCCESS;
}

Err RenderManager::RequestRender(const RenderRequest& request)
{
	// Create OpenGL Vertex Attribute Object and upload vertex data saving it in a list / use VAO list previously created
	if (attributeMap_.find(request.Model->ModelId) == attributeMap_.end())
	{
		std::vector<uint32_t> vaoList;

		for (uint32_t i = 0; i < request.Model->MeshCount; ++i)
		{
			uint32_t vao;

			Err err = VaoFactory::NewAttribObject(request.Model->Meshes[i], vao);
			if (err.Code())
				return err;

			vaoList.push_back(vao);

			RenderQuery newQuery = { {&request.Model->Meshes[i], vao}, request.Transform};
			renderQueue_.emplace(newQuery);
		}

		attributeMap_[request.Model->ModelId] = vaoList;
	}
	else
	{
		const std::vector<uint32_t>& vaoList = attributeMap_[request.Model->ModelId];

		for (uint32_t i = 0; i < request.Model->MeshCount; ++i)
		{
			RenderQuery newQuery = { {&request.Model->Meshes[i], vaoList[i]}, request.Transform};
			renderQueue_.emplace(newQuery);
		}
	}

	// Preload texture from meshes
	for (uint32_t i = 0; i < request.Model->MeshCount; ++i)
	{
		if (textures_.find(request.Model->Meshes[i].TextureAssetId) == textures_.end())
		{
			Err err = AddTexture(request.Model->Meshes[i].TextureAssetId);
			if (err.Code())
				GameConsoleManager::PrintError(err);
		}
	}

	return error_const::SUCCESS;
}

Err RenderManager::RequestBillboardRender(const BillboardRenderRequest& request)
{
	// Create OpenGL Vertex Attribute Object and upload vertex data saving it in a list / use VAO list previously created
	if (attributeMap_.find(request.Data.BillboardImageId) == attributeMap_.end())
	{
		uint32_t vao;

		Err err = VaoFactory::NewBillboardAttribObject(request.Data, vao);
		if (err.Code())
			return err;

		const std::vector<uint32_t> vaoList = { vao };

		RenderQuery newQuery = { {}, {}, request.Data, vao, true};
		renderQueue_.emplace(newQuery);

		attributeMap_[request.Data.BillboardImageId] = vaoList;
	}
	else
	{
		const uint32_t vao = attributeMap_[request.Data.BillboardImageId][0];

		RenderQuery newQuery = { {}, {}, request.Data, vao, true };
		renderQueue_.emplace(newQuery);
	}

	// Preload billboard texture
	if (textures_.find(request.Data.BillboardImageId) == textures_.end())
	{
		Err err = AddTexture(request.Data.BillboardImageId);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	return error_const::SUCCESS;
}

Err RenderManager::Draw()
{
	glViewport(viewport_.X, viewport_.Y, viewport_.Width, viewport_.Height);
	Drawer::Draw(renderQueue_, textures_);
	SDL_GL_SwapWindow(gameWindow_);

	return error_const::SUCCESS;
}

void RenderManager::ResizeViewport(const int32_t w, const int32_t h)
{
	viewport_ = { 0, 0, w, h };
}

Err RenderManager::AddTexture(const uint32_t assetId)
{
	Texture newTexture;

	Err err = newTexture.Init(assetId);
	if (err.Code())
		GameConsoleManager::PrintError(err.Message(), enums::ConsoleMessageSender::render);

	textures_[assetId] = newTexture;

	return error_const::SUCCESS;
}


SDL_Window* RenderManager::gameWindow_ = nullptr;
SDL_GLContext RenderManager::glContext_;
Viewport RenderManager::viewport_;

std::queue<RenderQuery> RenderManager::renderQueue_ = std::queue<RenderQuery>();
AttributeMap RenderManager::attributeMap_ = AttributeMap();
TextureList RenderManager::textures_ = TextureList();

Timeline RenderManager::renderTime_ = Timeline(timeline::MILLISECOND);