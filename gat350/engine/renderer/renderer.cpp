#include "renderer.h"

#include <glad/glad.h>

bool Renderer::Initialize(u32 width, u32 height, bool fullscreen) {
	Uint32 flags = SDL_WINDOW_OPENGL; 
	flags |= (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;

	window_ = SDL_CreateWindow("OpenGL", 100, 100, width, height, flags);
	if (!window_) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);

	context_ = SDL_GL_CreateContext(window_);
	if (!gladLoadGL()) {
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	return true;
}

void Renderer::Shutdown() {
	SDL_GL_DeleteContext(context_);
	SDL_DestroyWindow(window_);
}

void Renderer::ClearBuffer() {
	glClearColor(0.85f, 0.85f, 0.85f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffer() {
	SDL_GL_SwapWindow(window_);
}
