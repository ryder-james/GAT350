#pragma once
#include "../framework/system.h"

class Editor : public System {
public:
	OBJECT_DECLARATION(Editor, System)
	~Editor() {}

	bool Initialize();
	void Shutdown();
	void Update();

	void UpdateGUI();

public:
	bool visible_ = false;
	class Scene* scene_ = nullptr;
	class Actor* actor_ = nullptr;
};