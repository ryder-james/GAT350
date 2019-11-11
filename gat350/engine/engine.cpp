#include "engine.h"
#include "input/input.h"
#include "renderer/renderer.h"
#include "renderer/program.h"
#include "renderer/vertex_index_array.h"
#include "renderer/texture.h"
#include "renderer/material.h"
#include "renderer/light.h"
#include "renderer/mesh.h"
#include "renderer/model.h"
#include "renderer/gui.h"
#include "renderer/camera.h"

bool Engine::Initialize() {
	// core
	filesystem::set_current_path("assets");
	Name::AllocNames();

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// systems
	//std::unique_ptr<Input> input = std::make_unique<Input>(Input::GetClassName(), this);
	//input->Initialize();
	//systems_.push_back(std::move(input));

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(Renderer::GetClassName(), this);
	renderer->Initialize(1280, 720);
	systems_.push_back(std::move(renderer));

	GUI::Initialize(Get<Renderer>());

	// factory
	factory_ = std::make_unique<object_factory_t>();
	factory_->Register(Texture::GetClassName(), new Creator<Texture, Object>());
	factory_->Register(Model::GetClassName(), new Creator<Model, Object>());
	factory_->Register(Program::GetClassName(), new Creator<Program, Object>());
	factory_->Register(Material::GetClassName(), new Creator<Material, Object>());
	factory_->Register(Camera::GetClassName(), new Creator<Camera, Object>());
	factory_->Register(Light::GetClassName(), new Creator<Light, Object>());

	// resources
	resources_ = std::make_unique<resource_manager_t>();

	return true;
}

void Engine::Shutdown() {
	GUI::Shutdown();
	for (const std::unique_ptr<System>& system : systems_) {
		system->Shutdown();
	}

	SDL_Quit();
	Name::FreeNames();
}

void Engine::Update() {
	SDL_PollEvent(&event_);
	switch (event_.type) {
	case SDL_QUIT:
		quit_ = true;
		break;
	case SDL_KEYDOWN:
		if (event_.key.keysym.sym == SDLK_ESCAPE) {
			quit_ = true;
		}
	}
	SDL_PumpEvents();

	g_timer.tick();
	for (const std::unique_ptr<System>& system : systems_) {
		system->Update();
	}
}
