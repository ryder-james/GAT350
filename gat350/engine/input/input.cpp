#include "input.h"

bool Input::Initialize() {
	keyboard_state_ = SDL_GetKeyboardState(NULL);

	return true;
}

void Input::Shutdown() {}

void Input::Update() {}

bool Input::GetKey(SDL_Scancode scancode) {
	return keyboard_state_[scancode];
}
