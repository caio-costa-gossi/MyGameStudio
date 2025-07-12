#include "TestDrawer.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "WindowManager.h"

#include <glad/glad.h>

#include "SystemFileHelper.h"

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

	gameWindow_ = WindowManager::GetSdlWindow();

	err = InitRenderer();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TestDrawer::Run()
{
	Err err = SetupShader();
	if (err.Code())
	{
		GameConsoleManager::PrintCritical(err.Message(), enums::ConsoleMessageSender::render);
		return error_const::SUCCESS;
	}

	running_ = true;

	while (running_)
	{
		err = WindowManager::Update();
		if (err.Code() == error_const::EXIT_REQUEST_CODE)
			running_ = false;
		else if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::window);

		err = InputManager::Update();
		if (err.Code())
			GameConsoleManager::PrintError(err, enums::ConsoleMessageSender::input);

		err = Draw();
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

	GameConsoleManager::PrintInfo("Shutting down OpenGL context...", enums::ConsoleMessageSender::render);
	SDL_GL_DestroyContext(glContext_);

	return error_const::SUCCESS;
}

Err TestDrawer::InitRenderer()
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

Err TestDrawer::SetupShader()
{
	// Vertex data
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Setup Vertex Array Object
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Setup Vertex Buffer Object
	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setup Element Buffer Object
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	glUseProgram(shaderProgram_);

	// Define vertex attribute layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return error_const::SUCCESS;
}

Err TestDrawer::Draw()
{
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram_);
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	SDL_GL_SwapWindow(gameWindow_);

	return error_const::SUCCESS;
}

Err TestDrawer::LoadCompileSource(const char* sourcePath, const uint32_t shaderId)
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

Err TestDrawer::AttachLinkShaders(const uint32_t shaderProgram, const uint32_t vertexShader, const uint32_t fragShader)
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


void TestDrawer::ResizeViewport(const int32_t w, const int32_t h)
{
	glViewport(0, 0, w, h);
}


SDL_Window* TestDrawer::gameWindow_ = nullptr;
bool TestDrawer::running_ = false;
SDL_GLContext TestDrawer::glContext_;

double TestDrawer::counter_ = 0;
GLuint TestDrawer::shaderProgram_;
GLuint TestDrawer::vao_;