#pragma once
#include "../framework/system.h"

class Input : public System
{
public:
	enum eButtonState
	{
		IDLE,
		PRESSED,
		HELD,
		RELEASED
	};

	enum eDevice
	{
		KEYBOARD,
		MOUSE,
		CONTROLLER
	};

	enum eAxis
	{
		X,
		Y,
		Z
	};

	struct InputInfo
	{
		int id;
		eDevice device;
		int index;
	};

	struct ControllerInfo
	{
		SDL_GameController* controller;

		Uint8 button_state[SDL_CONTROLLER_BUTTON_MAX];
		Uint8 prev_button_state[SDL_CONTROLLER_BUTTON_MAX];
		float axis[SDL_CONTROLLER_AXIS_MAX];
		float prev_axis[SDL_CONTROLLER_AXIS_MAX];
	};

public:
	OBJECT_DECLARATION(Input, System)

	bool Initialize();
	void Shutdown();
	void Update();

	void AddAction(const Name& action, int id, eDevice device = KEYBOARD, int index = 0);
	
	eButtonState GetButton(const Name& action);
	bool GetButton(const Name& action, eButtonState state);
	float GetAxisAbsolute(const Name& action);
	float GetAxisRelative(const Name& action);

	eButtonState GetButtonState(int button, eDevice device = eDevice::KEYBOARD, int index = 0);
	float GetAxisAbsolute(eAxis axis, eDevice device = eDevice::MOUSE, size_t index = 0);
	float GetAxisRelative(eAxis axis, eDevice device = eDevice::MOUSE, size_t index = 0);

	void SetMouseRelative(bool enable = true);

protected:
	bool GetButtonDown(int button, eDevice device, int index = 0);
	bool GetPreviousButtonDown(int button, eDevice device, int index = 0);

private:
	// keyboard
	std::vector<Uint8> keystate_;
	std::vector<Uint8> prev_keystate_;
	int num_keys_;

	// mouse
	Uint32 mouse_buttonstate_;
	Uint32 prev_mouse_buttonstate_;
	glm::vec2 mouse_position_;
	glm::vec2 prev_mouse_position_;
	glm::vec2 mouse_relative_;
	bool mouse_relative_switch_;

	// controller
	std::vector<ControllerInfo> controllers_;

	// actions
	std::map<Name, InputInfo> actions_;
};
