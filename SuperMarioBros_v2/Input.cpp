#include "Input.h"

Input::Input()
{
}

void Input::update()
{
	lastFrameKeys.setMask(thisFrameKeys); // 1

	thisFrameKeys.setBit((int)Key::Left,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::A))); // 2

	thisFrameKeys.setBit((int)Key::Right,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::D)));

	thisFrameKeys.setBit((int)Key::Up,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::W)));

	thisFrameKeys.setBit((int)Key::Down,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::S)));

	thisFrameKeys.setBit((int)Key::Esc),
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

	thisFrameKeys.setBit((int)Key::Shift,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) );
}

bool Input::isKeyPressed(Key keycode)
{
	return thisFrameKeys.getBit((int)keycode);
}

bool Input::isKeyDown(Key keycode)
{
	bool lastFrame = lastFrameKeys.getBit((int)keycode);
	bool thisFrame = thisFrameKeys.getBit((int)keycode);

	return lastFrame && !thisFrame;
}

bool Input::isKeyUp(Key keycode)
{
	bool lastFrame = lastFrameKeys.getBit((int)keycode);
	bool thisFrame = thisFrameKeys.getBit((int)keycode);

	return !thisFrame && lastFrame;
}
