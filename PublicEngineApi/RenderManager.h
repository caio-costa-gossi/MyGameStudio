#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "Err.h"
#include "Mesh.h"
#include "Timeline.h"

class RenderManager
{
private:
	static SDL_GLContext glContext_;
	static SDL_Window* gameWindow_;
	static uint32_t shaderProgram_;

	static std::unordered_map<uint32_t, uint32_t> vertexAttributeConfigs_;
	static std::vector<Mesh> meshes_;

	static Timeline renderTime_;

	static Err InitRenderer();
	static Err SetupShader();

	static Err Draw();

	static Err LoadCompileSource(const char* sourcePath, uint32_t shaderId);
	static Err AttachLinkShaders(uint32_t shaderProgram, uint32_t vertexShader, uint32_t fragShader);
	static Err UpdateUniforms();

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err AddObject(const Mesh& mesh);
	static void ResizeViewport(int32_t w, int32_t h);
};
