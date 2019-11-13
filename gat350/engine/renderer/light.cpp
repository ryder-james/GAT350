#include "light.h"

#include "program.h"
#include "camera.h"

void Light::SetShader(class Program* shader) {
	ASSERT(shader);

	shader->Use();

	shader->SetUniform("light.ambient", ambient);
	shader->SetUniform("light.diffuse", diffuse);
	shader->SetUniform("light.specular", specular);

	std::vector<Camera*> cameras = scene_->Get<Camera>();

	ASSERT(!cameras.empty());
	
	glm::mat4 light_view_matrix = transform_.GetMatrix() * cameras[0]->view_matrix_;
	glm::vec3 p = light_view_matrix[3];
	shader->SetUniform("light.position", light_view_matrix[3]);
}

void Light::Edit() {
	ImGui::PushID("Light");
	ImGui::Text("Light");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::SliderFloat3("Position", (float*)&transform_.translation, -100.0f, 100.0f);
	ImGui::PopID();
}
