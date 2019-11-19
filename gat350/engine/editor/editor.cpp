#include "editor.h"

#include "../engine.h"
#include "../input/input.h"

bool Editor::Initialize() {
	engine_->Get<Input>()->AddAction("visible", SDL_SCANCODE_GRAVE, Input::KEYBOARD);

	return true;
}

void Editor::Shutdown() {
}

void Editor::Update() {
	if (engine_->Get<Input>()->GetButton("visible", Input::eButtonState::PRESSED)) visible_ = !visible_;
}

void Editor::UpdateGUI() {
	if (!visible_) return;

	if (!ImGui::CollapsingHeader("Scene")) return;

	std::vector<Actor*> actors = scene_->Get<Actor>();
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	for (Actor* actor : actors) {
		if (ImGui::TreeNodeEx(actor, node_flags, actor->name_.c_str())) {
			if (ImGui::IsItemClicked()) actor_ = actor;
		}
	}

	if (!ImGui::CollapsingHeader("Actor")) return;

	if (actor_) {
		actor_->Edit();
	}
}
