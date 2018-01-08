#pragma once

#include <SDL.h>
#include "Input.h"


class InputManager
{
public:
	// Create inputs
	Input forward, sideways, jump, fly, mouseSensitivity;
};
