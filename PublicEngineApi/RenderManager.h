#pragma once
#define SDL_PROP_WINDOW_WIND32_HWND_POINTER

#include <SDL3/SDL.h>

#include "BillboardRenderRequest.h"
#include "Err.h"
#include "RenderRequest.h"
#include "Timeline.h"
#include "Viewport.h"


class RenderManager
{
private:
	static SDL_GLContext glContext_;
	static SDL_Window* gameWindow_;
	static Viewport viewport_;

	static Timeline renderTime_;

	static Err InitRenderer();
	static Err Draw();

public:
	static Err Startup();
	static Err Update();
	static Err Shutdown();

	static Err RequestRender(const RenderRequest& request);
	static Err RequestBillboardRender(const BillboardRenderRequest& request);

	static void ResizeViewport(int32_t w, int32_t h);
};
