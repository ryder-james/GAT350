#pragma once
#include "object.h"
#include "../math/transform.h"

class Actor : public Object {
public:
	OBJECT_DECLARATION(Actor, Object)
	virtual ~Actor() {}

	Transform& GetTransform() { return transform_; }

	virtual void Edit() {
		Object::Edit();
		transform_.Edit();
	}

protected:
	Transform transform_;
};