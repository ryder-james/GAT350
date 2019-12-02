#include "bump_scene.h"
#include "fx_scene.h"
#include "cubemap_scene.h"
#include "multi_light_scene.h"

#include "../engine/engine.h"
#include "../engine/editor/editor.h"

int main(int argc, char** argv) {
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	engine->Initialize();

	std::unique_ptr<Scene> scene = std::make_unique<CubemapScene>(CubemapScene::GetClassName(), engine.get());
	scene->Create("scene");

	engine->Get<Editor>()->scene_ = scene.get();

	while (!engine->IsQuit()) {
		engine->Update();

		scene->Update();
		scene->Draw();
	}
	engine->Shutdown();

	return 0;
}
