#include "RenderManager.h"
#include "Enums.h"
#include "GameConsoleManager.h"
#include "SystemFileHelper.h"
#include "WindowManager.h"

Err RenderManager::Startup()
{
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

	return error_const::SUCCESS;
}

Err RenderManager::SetupShader()
{
	// Vertex Shader
	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	Err err = LoadCompileSource("shaders/MyShader.vert", vertexShader);
	if (err.Code())
		return err;

	// Fragment Shader
	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	err = LoadCompileSource("shaders/MyShader.frag", fragmentShader);
	if (err.Code())
		return err;

	// Linking Shaders and Building Program
	shaderProgram_ = glCreateProgram();
	err = AttachLinkShaders(shaderProgram_, vertexShader, fragmentShader);
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
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(mesh.VertexCount * sizeof(float) * 3), mesh.VertexList, GL_STATIC_DRAW);

	// Setup Element Buffer Object
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(mesh.IndexCount * sizeof(uint32_t)), mesh.IndexList, GL_STATIC_DRAW);

	// Define vertex attribute layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

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

	glUseProgram(shaderProgram_);

	for (uint32_t attributeConfig = 0; attributeConfig < vertexAttributeConfigs_.size(); ++attributeConfig)
	{
		glBindVertexArray(attributeConfig);
		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(meshes_[vertexAttributeConfigs_[attributeConfig]].IndexCount), GL_UNSIGNED_INT, nullptr);
	}
	
	SDL_GL_SwapWindow(gameWindow_);

	return error_const::SUCCESS;
}

Err RenderManager::LoadCompileSource(const char* sourcePath, const uint32_t shaderId)
{
	std::string vertexShaderSource;
	Err err = SystemFileHelper::ReadFileString(sourcePath, vertexShaderSource);
	if (err.Code())
		return err;

	const char* vertexSource = vertexShaderSource.data();
	glShaderSource(shaderId, 1, &vertexSource, nullptr);
	glCompileShader(shaderId);

	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
		return Err(infoLog, error_const::SHADER_COMPILATION_ERROR_CODE);
	}

	return error_const::SUCCESS;
}

Err RenderManager::AttachLinkShaders(const uint32_t shaderProgram, const uint32_t vertexShader, const uint32_t fragShader)
{
	// Attach and link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	// Delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	// Check if successful
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		return Err(infoLog, error_const::SHADER_LINKING_ERROR_CODE);
	}

	return error_const::SUCCESS;
}


void RenderManager::ResizeViewport(const int32_t w, const int32_t h)
{
	glViewport(0, 0, w, h);
}


SDL_Window* RenderManager::gameWindow_ = nullptr;
SDL_GLContext RenderManager::glContext_;
uint32_t RenderManager::shaderProgram_;

std::unordered_map<uint32_t,uint32_t> RenderManager::vertexAttributeConfigs_ = std::unordered_map<uint32_t, uint32_t>();
std::vector<Mesh> RenderManager::meshes_ = std::vector<Mesh>();