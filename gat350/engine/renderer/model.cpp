#include "model.h"
#include "camera.h"

void Model::Update() {}

void Model::Draw(GLenum primitiveType) {
	std::vector<Camera*> cameras = scene_->Get<Camera>();
	ASSERT(!cameras.empty());

	glm::mat4 model_view_matrix = cameras[0]->view_matrix_ * transform_.GetMatrix();
	glm::mat4 mvp_matrix = cameras[0]->projection_matrix_ * model_view_matrix;

	shader_->Use();
	shader_->SetUniform("mv_matrix", model_view_matrix);
	shader_->SetUniform("mvp_matrix", mvp_matrix);

	mesh_->SetShader(shader_.get());
	mesh_->Draw();
}
