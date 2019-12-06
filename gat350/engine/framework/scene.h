#pragma once
#include "actor.h"

class Scene : public Object {
public:
	OBJECT_DECLARATION(Scene, Object)
	virtual ~Scene() {}

	virtual void Update();
	virtual void Draw();

	void Add(std::unique_ptr<Actor> actor) {
		actors_.push_back(std::move(actor));
	}

	template<typename T>
	T* Get(const Name& name) {
		for (auto& actor : actors_) {
			if (actor->name_ == name) {
				return dynamic_cast<T*>(actor.get());
			}
		}

		return nullptr;
	}

	template<typename T>
	void SetActive(const Name& name) {
		for (auto& actor : actors_) {
			if (dynamic_cast<T*>(actor.get()))
				actor->active_ = (actor->name_ == name);
		}
	}

	template<typename T>
	T* GetActive() {
		for (auto& actor : actors_) {
			if (actor->active_ && dynamic_cast<T*>(actor.get())) {
				return dynamic_cast<T*>(actor.get());
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> Get() {
		std::vector<T*> actors;
		for (auto& actor : actors_) {
			if (dynamic_cast<T*>(actor.get()) != nullptr) {
				actors.push_back(dynamic_cast<T*>(actor.get()));
			}
		}

		return actors;
	}

protected:
	std::vector<std::unique_ptr<class Actor>> actors_;
};
