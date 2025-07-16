#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <queue>
#include <unordered_map>
#include <SDL3/SDL.h>

#include "Err.h"
#include "Mesh.h"
#include "MeshInstance.h"
#include "RenderQuery.h"
#include "RenderRequest.h"
#include "Shader.h"
#include "Texture.h"
#include "Timeline.h"

using AttributeMap = std::unordered_map<uint32_t, uint32_t>;
using TextureList = std::unordered_map<uint32_t, Texture>;

class RenderManager
{
private:
	static SDL_GLContext glContext_;
	static SDL_Window* gameWindow_;
	static Shader shader_;

	static std::queue<RenderQuery> renderQueue_;
	static AttributeMap attributeMap_;
	static TextureList textures_;

	static Timeline renderTime_;

	static Err InitRenderer();
	static Err SetupShader();

	static Err Draw();

	static Err NewAttribObject(const Mesh& mesh, uint32_t& newVaoId);
	static Err AddTexture(uint32_t assetId);

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err RequestRender(const RenderRequest& request);
	static void ResizeViewport(int32_t w, int32_t h);
};
