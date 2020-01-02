#pragma once
#include <SFML/Graphics.hpp>
#include "Bitmask.h"

class Input
{
public:
	Input();

	enum class Key
	{
		None = 0,
		Left = 1,
		Right = 2,
		Up = 3,
		Down = 4,
		Esc = 5
	};
	
	bool isKeyPressed(Key keycode);
	bool isKeyDown(Key keycode);
	bool isKeyUp(Key keycode);

	void update();

private:
	Bitmask thisFrameKeys;
	Bitmask lastFrameKeys;
};

