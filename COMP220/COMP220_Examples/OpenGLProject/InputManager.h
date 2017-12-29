#pragma once

#include <SDL.h>
#include "Input.h"


class InputManager
{
public:

	InputManager();
	~InputManager();

	Input forward, sideways, jump, fly;
};
