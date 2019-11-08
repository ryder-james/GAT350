#pragma once

#include "..//engine.h"

class Renderer {
public:
	Renderer() {}
	~Renderer() {}

	bool Initialize(u32 width, u32 height, bool fullscreen = false);
	void Shutdown();

	void ClearBuffer();
	void SwapBuffer();

	SDL_Window* GetWindow() { return window_; }
	SDL_GLContext GetContext() { return context_; }

private:
	SDL_Window* window_ = nullptr;
	SDL_GLContext context_;
};
