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
		ImGui::Checkbox("Active", &active_);
		transform_.Edit();
	}

public:
	bool active_ = true;
	Transform transform_;
	class Scene* scene_ = nullptr;
};