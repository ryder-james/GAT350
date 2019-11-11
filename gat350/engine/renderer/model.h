#pragma once
#include "../framework/actor.h"
#include "mesh.h"
#include "program.h"

class Model : public Actor {
public:
	OBJECT_DECLARATION(Model, Actor)
		~Model() {}

	void Update() override;
	void Draw(GLenum primitiveType = GL_TRIANGLES) override;

public:
	std::shared_ptr<Mesh> mesh_;
	std::shared_ptr<Program> shader_;
};
