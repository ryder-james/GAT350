#include "camera.h"

#include "../input/input.h"
#include "../engine.h"

bool Camera::Create(const Name& name) {
	name_ = name;

	engine_->Get<Input>()->AddAction("rotate_camera_x", Input::X, Input::MOUSE);
	engine_->Get<Input>()->AddAction("rotate_camera_y", Input::Y, Input::MOUSE);

	engine_->Get<Input>()->AddAction("camera_forward", SDL_SCANCODE_W);
	engine_->Get<Input>()->AddAction("camera_backward", SDL_SCANCODE_S);

	return true;
}

void Camera::Update() {
	glm::vec3 rotation(0);
	if (engine_->Get<Input>()->GetButton("camera_orbit", Input::HELD))
	{
		rotation.x = engine_->Get<Input>()->GetAxisRelative("rotate_camera_x") * 0.01f;
		rotation.y = engine_->Get<Input>()->GetAxisRelative("rotate_camera_y") * 0.01f;
	}

	glm::quat pitch = glm::angleAxis(rotation.y, glm::vec3(1, 0, 0));
	glm::quat yaw = glm::angleAxis(rotation.x, glm::vec3(0, 1, 0));

	transform_.rotation = pitch * transform_.rotation * yaw;

	glm::vec3 target = transform_.translation + transform_.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	view_matrix_ = glm::lookAt(transform_.translation, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetProjection(float fov, float aspect_ratio, float near_clip, float far_clip) {
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	this->near_clip = near_clip;
	this->far_clip = far_clip;

	projection_matrix_ = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}
