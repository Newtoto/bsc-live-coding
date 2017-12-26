#pragma once
#include <fstream>

class Input
{
public:
	Input();
	~Input();

	void SetPositive();
	void SetNegative();
	void ZeroNegative();
	void ZeroPositive();

	float GetValue();

	bool positivePressed;
	bool negativePressed;

};

