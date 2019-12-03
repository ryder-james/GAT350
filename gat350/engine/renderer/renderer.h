#pragma once

#include "../framework/system.h"

class Renderer : public System {
public:
	OBJECT_DECLARATION(Renderer, System)
	~Renderer() {}

	bool Initialize();
	bool Initialize(u32 width, u32 height, bool fullscreen = false);
	void Shutdown();
	void Update();

	void ClearBuffer();
	void SwapBuffer();

	SDL_Window* GetWindow() { return window_; }
	SDL_GLContext GetContext() { return context_; }

	void SetViewport(u32 x, u32 y, u32 width, u32 height) { glViewport(x, y, width, height); }
	void RestoreViewport() { glViewport(0, 0, width_, height_); }

	u32 GetWidth() { return width_; }
	u32 GetHeight() { return height_; }

private:
	SDL_Window* window_ = nullptr;
	SDL_GLContext context_;

	u32 width_;
	u32 height_;
};
