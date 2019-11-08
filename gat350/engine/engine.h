#pragma once

#include "def.h"
#include "framework/object.h"
#include "framework/scene.h"
#include "framework/system.h"

class Engine : public Object {
public:
	OBJECT_DECLARATION(Engine, Object)
	virtual ~Engine() {}

	bool Initialize();
	void Shutdown();
	void Update();

	bool IsQuit() { return quit_; }

	template <typename T>
	T* Get() {
		for (const auto& system : systems_) {
			if (dynamic_cast<T*>(system.get()) != nullptr) {
				return static_cast<T*>(system.get());
			}
		}

		return nullptr;
	}

	object_factory_t* Factory() const { return factory_.get(); }
	resource_manager_t* Resources() const { return resources_.get(); }

	SDL_Event& GetEvent() { return event_; }

protected:
	bool quit_ = false;
	SDL_Event event_;

	std::vector<std::unique_ptr<class System>> systems_;
	std::unique_ptr<object_factory_t> factory_;
	std::unique_ptr<resource_manager_t> resources_;
};
