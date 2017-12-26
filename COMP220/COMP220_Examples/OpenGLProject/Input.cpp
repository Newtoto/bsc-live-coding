#include "Input.h"



Input::Input()
{
	negativePressed = false;
	positivePressed = false;
}


Input::~Input()
{
}

void Input::SetPositive()
{
	if (!negativePressed)
	{
		positivePressed = true;
	}
}

void Input::SetNegative()
{
	if (!positivePressed)
	{
		negativePressed = true;
	}
}

void Input::ZeroPositive()
{
	if (GetValue() == 1.0f)
	{
		positivePressed = false;
	}
}

void Input::ZeroNegative()
{
	if (GetValue() == -1.0f)
	{
		negativePressed = false;
	}
}

float Input::GetValue()
{
	if (positivePressed)
	{
		return 1.0f;
	}
	else if (negativePressed)
	{
		return -1.0f;
	}
	else
	{
		return 0;
	}
}