#pragma once
#include "../def.h"
#include "object.h"

//#define SYSTEM_DECLARATION(class_name, super_class_name) \
//	class_name(class Engine* engine) : super_class_name(engine) {} \
//	virtual ~class_name() {} \
//	virtual const char* GetClassName() { return #class_name; }

class System : public Object {
public:
	OBJECT_DECLARATION(System, Object)
		virtual ~System() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
};
