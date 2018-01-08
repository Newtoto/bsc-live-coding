#pragma once
#include <fstream>

class Input
{
public:
	Input();
	~Input();

	// Set input value to positive
	void SetPositive();
	// Set input value to negative
	void SetNegative();
	// If input is negative, set to 0
	void ZeroNegative();
	// If input is positive, set to 0
	void ZeroPositive();

	// Return current held value of the input
	float GetValue();

	bool positivePressed;
	bool negativePressed;

};

