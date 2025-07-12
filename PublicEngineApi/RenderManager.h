#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "Err.h"

class RenderManager
{
private:
	static SDL_GLContext glContext_;
	static SDL_Window* gameWindow_;

	static GLuint shaderProgram_;
	static GLuint vao_;

	static Err InitRenderer();
	static Err SetupShader();

	static Err Draw();

	static Err LoadCompileSource(const char* sourcePath, uint32_t shaderId);
	static Err AttachLinkShaders(uint32_t shaderProgram, uint32_t vertexShader, uint32_t fragShader);

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static void ResizeViewport(int32_t w, int32_t h);
};
