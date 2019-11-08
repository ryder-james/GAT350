#pragma once

#include "../framework/system.h"

class Input : public System {
public:
	Input() {}
	~Input() {}

	bool Initialize();
	void Shutdown();

	void Update();

	bool GetKey(SDL_Scancode scancode);

private:
	const Uint8* keyboard_state_ = nullptr;
};
