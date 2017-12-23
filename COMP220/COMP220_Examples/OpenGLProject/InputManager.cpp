#include "InputManager.h"



InputManager::InputManager()
{
	// Set all inputs to 0
	int forward = 0;
	int sideways = 0;
	int mouseSensitivityUp = 0;
}


InputManager::~InputManager()
{

}

void InputManager::ZeroValue(int& input, int inputValue)
{
	// Sets value of input to 0 if it has been pressed
	if (input == inputValue) {
		input = 0;
	}
}
