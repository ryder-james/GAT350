#pragma once
#include "../def.h"
#include "../../framework/factory.h"

#define OBJECT_DECLARATION(class_name, super_class_name) \
	class_name() {} \
	class_name(const Name& name, class Engine* engine) : super_class_name(name, engine) {} \
	static const char* GetClassName() { return #class_name; } \
	static const char* GetSuperClassName() { return #super_class_name; }

class Object {
public:
	Object() {}
	Object(const Name& name, class Engine* engine) : name_(name), engine_(engine) {}
	virtual ~Object() {}

	virtual bool Create(const Name& name) { name_ = name; return true; }

	inline virtual void Edit() { ImGui::Text(name_.c_str()); }

public:
	Name name_;
	class Engine* engine_ = nullptr;
};
