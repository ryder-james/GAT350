#include "model.h"
#include "camera.h"

void Model::Update() {}

void Model::Draw(GLenum primitiveType) {
	Camera* camera = scene_->GetActive<Camera>();
	//ASSERT(!cameras.empty());

	glm::mat4 model_view_matrix = camera->view_matrix_ * transform_.GetMatrix();
	glm::mat4 mvp_matrix = camera->projection_matrix_ * model_view_matrix;
	glm::mat4 view_matrix = glm::mat4(glm::mat3(camera->view_matrix_));
	glm::mat4 projection_matrix = camera->projection_matrix_;

	shader_->Use();

	shader_->SetUniform("camera_position", camera->transform_.translation);
	shader_->SetUniform("model_matrix", transform_.GetMatrix());
	shader_->SetUniform("mv_matrix", model_view_matrix);
	shader_->SetUniform("mvp_matrix", mvp_matrix);
	shader_->SetUniform("view_matrix", view_matrix);
	shader_->SetUniform("projection_matrix", projection_matrix);

	mesh_->SetShader(shader_.get());
	mesh_->Draw();
}
