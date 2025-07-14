#include "RenderManager.h"

#include <glad/glad.h>

#include "AssetRuntimeManager.h"
#include "Color.h"
#include "Enums.h"
#include "GameConsoleManager.h"
#include "Image.h"
#include "ImageLoader.h"
#include "MVector.h"
#include "Texture.h"
#include "WindowManager.h"

Err RenderManager::Startup()
{
	renderTime_.Start();

	gameWindow_ = WindowManager::GetSdlWindow();

	Err err = InitRenderer();
	if (err.Code())
		return err;

	err = SetupShader();
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
	glViewport(0, 0, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());

	Err err = GenerateTexture();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::SetupShader()
{
	Err err = shader_.Init("shaders/MyShader.vert", "shaders/MyShader.frag");
	if (err.Code())
		return err;

	err = shader_.Build();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err RenderManager::AddObject(const Mesh& mesh)
{
	// Setup Vertex Array Object
	uint32_t newVao;
	glGenVertexArrays(1, &newVao);
	glBindVertexArray(newVao);

	// Setup Vertex Buffer Object
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(mesh.VertexCount * sizeof(float) * 10), mesh.VertexList, GL_STATIC_DRAW);

	// Setup Element Buffer Object
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(mesh.IndexCount * sizeof(uint32_t)), mesh.IndexList, GL_STATIC_DRAW);

	// Define vertex attribute layout
	// Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TextCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (8 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Save Mesh data & VAO Index
	meshes_.emplace_back(mesh);
	vertexAttributeConfigs_[newVao] = static_cast<uint32_t>(meshes_.size() - 1);

	return error_const::SUCCESS;
}

Err RenderManager::Draw()
{
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader_.Use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	UpdateUniforms();

	for (uint32_t attributeConfig = 0; attributeConfig < vertexAttributeConfigs_.size(); ++attributeConfig)
	{
		glBindVertexArray(attributeConfig);
		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(meshes_[vertexAttributeConfigs_[attributeConfig]].IndexCount), GL_UNSIGNED_INT, nullptr);
	}
	
	SDL_GL_SwapWindow(gameWindow_);

	return error_const::SUCCESS;
}

Err RenderManager::UpdateUniforms()
{
	const float elapsed = static_cast<float>(renderTime_.GetElapsed());
	const Vec3F posOffset = { sin(elapsed / 1000), 0, 0 };
	shader_.SetUniform("posOffset", posOffset.X, posOffset.Y, posOffset.Z);

	return error_const::SUCCESS;
}

void RenderManager::ResizeViewport(const int32_t w, const int32_t h)
{
	glViewport(0, 0, w, h);
}

Err RenderManager::GenerateTexture()
{
	Texture myTexture;

	Err err = myTexture.Init(46);
	if (err.Code())
		GameConsoleManager::PrintError(err.Message(), enums::ConsoleMessageSender::render);
	else
		myTexture.Use();

	return error_const::SUCCESS;
}


SDL_Window* RenderManager::gameWindow_ = nullptr;
SDL_GLContext RenderManager::glContext_;
Shader RenderManager::shader_;

std::unordered_map<uint32_t,uint32_t> RenderManager::vertexAttributeConfigs_ = std::unordered_map<uint32_t, uint32_t>();
std::vector<Mesh> RenderManager::meshes_ = std::vector<Mesh>();

Timeline RenderManager::renderTime_ = Timeline(timeline::MILLISECOND);