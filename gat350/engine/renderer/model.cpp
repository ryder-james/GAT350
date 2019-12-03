#include "model.h"
#include "camera.h"

void Model::Update() {}

void Model::Draw(GLenum primitiveType) {
	std::vector<Camera*> cameras = scene_->Get<Camera>();
	ASSERT(!cameras.empty());

	glm::mat4 model_view_matrix = cameras[0]->view_matrix_ * transform_.GetMatrix();
	glm::mat4 mvp_matrix = cameras[0]->projection_matrix_ * model_view_matrix;
	glm::mat4 view_matrix = glm::mat4(glm::mat3(cameras[0]->view_matrix_));
	glm::mat4 projection_matrix = cameras[0]->projection_matrix_;

	shader_->Use();

	shader_->SetUniform("camera_position", cameras[0]->transform_.translation);
	shader_->SetUniform("model_matrix", transform_.GetMatrix());
	shader_->SetUniform("mv_matrix", model_view_matrix);
	shader_->SetUniform("mvp_matrix", mvp_matrix);
	shader_->SetUniform("view_matrix", view_matrix);
	shader_->SetUniform("projection_matrix", projection_matrix);

	mesh_->SetShader(shader_.get());
	mesh_->Draw();
}
