#include "light.h"
#include "program.h"
#include "camera.h"

bool Light::Create(const Name& name)
{
	name_ = name;

#ifdef _DEBUG
	debug_model_ = engine_->Factory()->Create<Model>(Model::GetClassName());
	debug_model_->name_ = "model";
	debug_model_->engine_ = engine_;
	debug_model_->scene_ = scene_;
	debug_model_->transform_.scale = glm::vec3(0.15f, 0.15f, 0.15f);
	debug_model_->mesh_ = engine_->Resources()->Get<Mesh>("meshes/cube.obj");
	debug_model_->mesh_->material_ = engine_->Resources()->Get<Material>("debug_material");
	debug_model_->shader_ = engine_->Resources()->Get<Program>("debug_shader");
#endif // _DEBUG

	return true;
}

void Light::Update()
{
}

void Light::Draw(GLenum primitiveType)
{
#ifdef _DEBUG
	debug_model_->transform_.translation = transform_.translation;
	debug_model_->transform_.rotation = transform_.rotation;
	debug_model_->mesh_->material_->diffuse = diffuse;
	debug_model_->Draw(primitiveType);
#endif // _DEBUG
}

void Light::SetShader(class Program* shader)
{
	SetShader("light", shader);
}

void Light::SetShader(const std::string& lightname, Program* shader)
{
	ASSERT(shader);

	shader->Use();
	shader->SetUniform(lightname + ".ambient", ambient);
	shader->SetUniform(lightname + ".diffuse", diffuse);
	shader->SetUniform(lightname + ".specular", specular);
	shader->SetUniform(lightname + ".type", type);
	shader->SetUniform(lightname + ".cutoff", glm::radians(cutoff));
	shader->SetUniform(lightname + ".exponent", exponent);

	std::vector<Camera*> cameras = scene_->Get<Camera>();
	ASSERT(!cameras.empty());

	glm::mat4 light_view_matrix = cameras[0]->view_matrix_ * transform_.GetMatrix();
	shader->SetUniform("light.position", light_view_matrix[3]);
	shader->SetUniform("light.direction", glm::mat3(light_view_matrix) * glm::vec3(0.0f, 0.0f, 1.0f));
}

void Light::Edit()
{
	Actor::Edit();

	ImGui::Separator();

	ImGui::Text("Type: %s", Light::GetClassName());
	ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
	ImGui::SliderFloat("Cutoff", &cutoff, 0, 90);
	ImGui::SliderFloat("Exponent", &exponent, 0, 128);

	const char* types[] = { "Point", "Direction", "Spot" };
	ImGui::Combo("Type", (int*)&type, types, IM_ARRAYSIZE(types));
}
