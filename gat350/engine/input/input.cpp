#include "input.h"

bool Input::Initialize()
{
	// create keyboard state buffers
	SDL_GetKeyboardState(&num_keys_);
	keystate_.resize(num_keys_);
	prev_keystate_.resize(num_keys_);

	// set initial keyboard states
	const Uint8* keystate = SDL_GetKeyboardState(nullptr);
	memcpy(keystate_.data(), keystate, num_keys_);
	memcpy(prev_keystate_.data(), keystate_.data(), num_keys_);

	// set initial mouse buttons states
	SDL_Point axis;
	mouse_buttonstate_ = SDL_GetMouseState(&axis.x, &axis.y);
	prev_mouse_buttonstate_ = mouse_buttonstate_;
	mouse_position_ = glm::vec2(axis.x, axis.y);
	prev_mouse_position_ = mouse_position_;

	// query controllers attached, add controller info to controllers
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			ControllerInfo controllerInfo;
			controllerInfo.controller = SDL_GameControllerOpen(i);
			memset(controllerInfo.button_state, 0, SDL_CONTROLLER_BUTTON_MAX);
			memset(controllerInfo.prev_button_state, 0, SDL_CONTROLLER_BUTTON_MAX);
			memset(controllerInfo.axis, 0, SDL_CONTROLLER_AXIS_MAX);
			memset(controllerInfo.prev_axis, 0, SDL_CONTROLLER_AXIS_MAX);

			controllers_.push_back(controllerInfo);
		}
	}

	return true;
}

void Input::Shutdown()
{
	//
}

void Input::Update()
{
	// set previous keyboard state
	memcpy(prev_keystate_.data(), keystate_.data(), num_keys_);
	// get current keyboard state
	const Uint8* keystate = SDL_GetKeyboardState(nullptr);
	memcpy(keystate_.data(), keystate, num_keys_);

	// set previous mouse state
	prev_mouse_buttonstate_ = mouse_buttonstate_;
	prev_mouse_position_ = mouse_position_;
	// get current mouse state
	SDL_Point axis;
	mouse_buttonstate_ = SDL_GetMouseState(&axis.x, &axis.y);
	mouse_position_ = glm::vec2(axis.x, axis.y);
	// get relative mouse state
	if (SDL_GetRelativeMouseMode())
	{
		mouse_buttonstate_ = SDL_GetRelativeMouseState(&axis.x, &axis.y);
		mouse_relative_ = (mouse_relative_switch_) ? glm::vec2(0, 0) : glm::vec2(axis.x, axis.y);
		mouse_relative_switch_ = false;
	}

	// update controllers
	for (ControllerInfo& controllerInfo : controllers_)
	{
		// controller buttons
		memcpy(controllerInfo.prev_button_state, controllerInfo.button_state, SDL_CONTROLLER_BUTTON_MAX);
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
		{
			controllerInfo.button_state[i] = SDL_GameControllerGetButton(controllerInfo.controller, (SDL_GameControllerButton)i);
		}

		// controller axis
		memcpy(controllerInfo.prev_axis, controllerInfo.axis, SDL_CONTROLLER_AXIS_MAX);
		for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
		{
			Sint16 value = SDL_GameControllerGetAxis(controllerInfo.controller, (SDL_GameControllerAxis)i);
			controllerInfo.axis[i] = value / float(SDL_MAX_SINT16);
		}
	}
}

void Input::AddAction(const Name& action, int id, eDevice device, int index)
{
	auto iter = actions_.find(action);
	ASSERT(iter == actions_.end());

	if (iter == actions_.end())
	{
		InputInfo inputInfo = { id, device, index };
		actions_[action] = inputInfo;
	}
}

Input::eButtonState Input::GetButton(const Name& action)
{
	eButtonState state = eButtonState::IDLE;

	auto iter = actions_.find(action);
	if (iter != actions_.end())
	{
		InputInfo inputInfo = iter->second;
		state = GetButtonState(inputInfo.id, inputInfo.device, inputInfo.index);
	}

	return state;
}

bool Input::GetButton(const Name& action, eButtonState state)
{
	return GetButton(action) == state;
}

float Input::GetAxisAbsolute(const Name& action)
{
	float axis = 0.0f;

	auto iter = actions_.find(action);
	if (iter != actions_.end())
	{
		InputInfo inputInfo = iter->second;
		axis = GetAxisAbsolute(static_cast<eAxis>(inputInfo.id), inputInfo.device, inputInfo.index);
	}

	return axis;
}

float Input::GetAxisRelative(const Name& action)
{
	float axis = 0.0f;

	auto iter = actions_.find(action);
	if (iter != actions_.end())
	{
		InputInfo inputInfo = iter->second;
		axis = GetAxisRelative(static_cast<eAxis>(inputInfo.id), inputInfo.device, inputInfo.index);
	}

	return axis;
}

Input::eButtonState Input::GetButtonState(int button, eDevice device, int index)
{
	eButtonState state = eButtonState::IDLE;

	bool button_down = GetButtonDown(button, device, index);
	bool prev_button_down = GetPreviousButtonDown(button, device, index);

	if (button_down)
	{
		state = (prev_button_down) ? eButtonState::HELD : eButtonState::PRESSED;
	}
	else
	{
		state = (prev_button_down) ? eButtonState::RELEASED : eButtonState::IDLE;
	}

	return state;
}

float Input::GetAxisAbsolute(eAxis axis, eDevice device, size_t index)
{
	float axis_value = 0.0f;

	switch (device)
	{
	case eDevice::KEYBOARD:
		ASSERT_MSG(0, "Keyboard does not support axis.");
		break;

	case eDevice::MOUSE:
		axis_value = mouse_position_[axis];
		break;

	case eDevice::CONTROLLER:
		ASSERT_MSG(index < controllers_.size(), "Invalid controller index.");
		axis_value = controllers_[index].axis[axis];
		break;
	}

	return axis_value;
}

float Input::GetAxisRelative(eAxis axis, eDevice device, size_t index)
{
	float axis_value = 0.0f;

	switch (device)
	{
	case eDevice::KEYBOARD:
		ASSERT_MSG(0, "Keyboard does not support axis.");
		break;

	case eDevice::MOUSE:
		axis_value = (SDL_GetRelativeMouseMode()) ? mouse_relative_[axis] : mouse_position_[axis] - prev_mouse_position_[axis];
		break;

	case eDevice::CONTROLLER:
		ASSERT_MSG(index < controllers_.size(), "Invalid controller index.");
		axis_value = controllers_[index].axis[axis] - controllers_[index].prev_axis[axis];
		break;
	}

	return axis_value;
}

void Input::SetMouseRelative(bool enable)
{
	SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
	mouse_relative_switch_ = true;
}

bool Input::GetButtonDown(int button, eDevice device, int index)
{
	bool button_down = false;

	switch (device)
	{
	case eDevice::KEYBOARD:
		button_down = keystate_[button];
		break;

	case eDevice::MOUSE:
		button_down = mouse_buttonstate_ & SDL_BUTTON(button);
		break;

	case eDevice::CONTROLLER:
		ASSERT_MSG(index < controllers_.size(), "Invalid controller index.");
		button_down = controllers_[index].button_state[button];
		break;
	}

	return button_down;
}

bool Input::GetPreviousButtonDown(int button, eDevice device, int index)
{
	bool button_down = false;

	switch (device)
	{
	case eDevice::KEYBOARD:
		button_down = prev_keystate_[button];
		break;

	case eDevice::MOUSE:
		button_down = prev_mouse_buttonstate_ & SDL_BUTTON(button);
		break;

	case eDevice::CONTROLLER:
		assert(index < controllers_.size());
		button_down = controllers_[index].prev_button_state[button];
		break;
	}

	return button_down;
}
