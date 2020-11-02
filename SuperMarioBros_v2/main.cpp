#include "SFML/Audio.hpp"
#include "Game.h"
#include <iostream>

int main()
{
	Game game;

	/*sf::Music soundtrack;
	if (!(soundtrack.openFromFile("../Resources/sound.ogg")))
	{
		std::cout << "Sound error.";
	}*/
	
	//soundtrack.play();

	while (game.isRunning())
	{
		game.processInput();
		game.update();
		game.lateUpdate();
		game.draw();	
		game.calculateDeltaTime();
	}
}