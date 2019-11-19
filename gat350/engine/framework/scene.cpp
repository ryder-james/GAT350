#include "scene.h"

void Scene::Update() {
	for (const auto& actor : actors_) {
		if (actor->active_) actor.get()->Update();
	}
}

void Scene::Draw() {
	for (const auto& actor : actors_) {
		if (actor->active_) actor.get()->Draw();
	}
}
