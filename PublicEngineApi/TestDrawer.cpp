#include "TestDrawer.h"
#include "GameConsoleManager.h"
#include "InputManager.h"
#include "WindowManager.h"

#include <glad/glad.h>

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
		return err;

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
	const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
	)";

	const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.3, 0.2, 1.0); // Red-ish color
    }
	)";

	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Create shader program
	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader);
	glAttachShader(shaderProgram_, fragmentShader);
	glLinkProgram(shaderProgram_);

	// Clean up shaders (they’re in the program now)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex data
	float vertices[] = {
		 0.0f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	// Setup VAO and VBO
	GLuint VBO;
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &VBO);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Describe vertex layout
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind to be safe
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return error_const::SUCCESS;
}

Err TestDrawer::Draw()
{
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram_);
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(gameWindow_);

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