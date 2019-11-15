#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"
#include "ResourceHolder.cpp"
#include "Mario.h"

namespace tex
{
	enum ID { Mario, Blocks, Enemy, Powerups };
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SuperMarioBros");
	sf::RenderStates states;

	sf::Clock clock;
	float deltaTime;

	ResourceHolder<sf::Texture, tex::ID> resourceHolderTexture;
	resourceHolderTexture.load(tex::Mario, "../Resources/mario.png");

	Mario mario(resourceHolderTexture.get(tex::Mario), sf::Vector2u(3, 4), 250.0f, 10.0f, 0.065f);

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == event.KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		window.clear(sf::Color::Black);

		mario.Update(deltaTime);

		mario.draw(window, states);

		window.display();

	}

	return 0;
}