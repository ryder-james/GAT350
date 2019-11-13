#pragma once

#include "../framework/actor.h"
#include "../engine.h"

class Light : public Actor {
public:
	OBJECT_DECLARATION(Light, Actor)
	virtual ~Light() {}

	virtual bool Create(const Name& name) { name_ = name; return true; }

	void SetShader(class Program* shader);
	void Edit();

public:
	//union {
	//	glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//	glm::vec4 direction;
	//};

	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
};