#pragma once
#include "../engine/framework/scene.h"

class LightScene : public Scene
{
public:
	OBJECT_DECLARATION(LightScene, Scene);

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;

public:
	glm::vec3 scale_ = glm::vec3(1);

	float time_ = 0.0f;
	float rate_ = 1.0f;
	float amplitude_ = 0.0f;
	float frequency_ = 0.0f;

	glm::vec2 uv_scale_ = glm::vec2(1);
	glm::vec2 uv_offset_ = glm::vec2(0);
};