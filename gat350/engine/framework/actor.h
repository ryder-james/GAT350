#pragma once
#include "object.h"
#include "../math/transform.h"

class Actor : public Object {
public:
	OBJECT_DECLARATION(Actor, Object)
	virtual ~Actor() {}

	virtual void Update() {}
	virtual void Draw(GLenum primitive_type = GL_TRIANGLES) {}

	virtual void Edit() {
		Object::Edit();
		transform_.Edit();
	}

public:
	Transform transform_;
	class Scene* scene_ = nullptr;
};