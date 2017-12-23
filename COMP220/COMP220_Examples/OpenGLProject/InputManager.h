#pragma once

#include <SDL.h>


class InputManager
{
public:

	InputManager();
	~InputManager();

	int forward, sideways, mouseSensitivityUp;

	void ZeroValue(int& input, int inputValue);
};
