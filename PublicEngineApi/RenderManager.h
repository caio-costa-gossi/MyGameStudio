#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>

#include "Err.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Timeline.h"

using TextureList = std::unordered_map<uint32_t, Texture>;

class RenderManager
{
private:
	static SDL_GLContext glContext_;
	static SDL_Window* gameWindow_;
	static Shader shader_;

	static std::unordered_map<uint32_t, uint32_t> vertexAttributeConfigs_;
	static TextureList textures_;
	static std::vector<Mesh> meshes_;

	static Timeline renderTime_;

	static Err InitRenderer();
	static Err SetupShader();

	static Err Draw();

	static Err UpdateUniforms();
	static Err AddTexture(uint32_t assetId);

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err AddMesh(const Mesh& mesh);
	static void ResizeViewport(int32_t w, int32_t h);
};
