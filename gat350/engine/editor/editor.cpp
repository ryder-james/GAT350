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

	if (!ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) return;

	std::vector<Actor*> actors = scene_->Get<Actor>();
	for (Actor* actor : actors) {
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (actor == actor_) node_flags |= ImGuiTreeNodeFlags_Selected;

		ImColor color = (actor->active_ ? ImColor(1.0f, 1.0f, 1.0f) : ImColor(0.5f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color));


		if (ImGui::TreeNodeEx(actor, node_flags, actor->name_.c_str())) {
			
			if (ImGui::IsItemClicked()) actor_ = actor;
		}
		ImGui::PopStyleColor();
	}

	if (!ImGui::CollapsingHeader("Actor", ImGuiTreeNodeFlags_DefaultOpen)) return;

	if (actor_) {
		actor_->Edit();
	}
}
