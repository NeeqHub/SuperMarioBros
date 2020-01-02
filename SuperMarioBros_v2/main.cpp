/*#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include "ResourceHolder.h"
#include "ResourceHolder.cpp"
#include "Mario.h"
#include "Blocks.h"

//To delete later
class NotImplemented : public std::logic_error
{
public:
	NotImplemented() : std::logic_error("Function not yet implemented") { };
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SuperMarioBros");
	sf::RenderStates states;
	sf::View view(Vector2f(0.0f, 0.0f), Vector2f(1920.0f, 1080.0f));

	sf::Clock clock;
	float deltaTime;

	ResourceHolder<sf::Texture, tex::ID> resourceHolderTexture;
	resourceHolderTexture.load(tex::Mario, "../Resources/mario.png");
	resourceHolderTexture.load(tex::Blocks, "../Resources/blocks.png");

	//Mario mario(resourceHolderTexture.get(tex::Mario), sf::Vector2u(3, 4), 500.0f, 10.0f, 0.065f);
	//std::shared_ptr<Mario> mario;
	//mario = make_shared<Mario>(resourceHolderTexture.get(tex::Mario), sf::Vector2u(3, 4), 500.0f, 10.0f, 0.065f);

	std::shared_ptr<GameObject> mario;
	mario = make_shared<GameObject>();

	auto graphics = mario->addComponent<CGraphics>();
	graphics->loadTexture(tex::Mario, "../Resources/mario.png");
	graphics->setTexture(tex::Mario);

	auto transform = mario->getComponent<CTransform>();
	transform->setPosition(sf::Vector2f(0.0f, 0.0f));

	std::vector<Blocks> blocks;

	std::ifstream blocksCoordinates;
	blocksCoordinates.open("../Resources/blocksCoordinatesStage1.txt");

	if (!blocksCoordinates.good())
		throw new NotImplemented();

	float x, y;
	int type;

	while (true)
	{
		if (!blocksCoordinates.fail()) //Read to end of file
		{
			blocksCoordinates >> x >> y >> type;
			cout << x << y << type << endl;
			blocks.push_back(Blocks(resourceHolderTexture.get(tex::Blocks), sf::Vector2u(16, 16), 64, 16, Vector2f(x * 48.0f, y * 48.0f), type));		
		}
		else
		{
			blocksCoordinates.close();
			break;
		}
			
	}
	
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event event;

		window.clear(sf::Color::Black);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == event.KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		//view.setCenter();
		//window.setView(view);

		/*mario->Update(deltaTime);

		Vector2f direction;

		for (Blocks& block : blocks)
		{
			if (block.GetCollider().CheckCollision(mario->getCollider(), direction, 0.5f))
			{
				//if (block.tag == "questionBlock" && direction.y > 0.0f)

				if (block.tag == "brick" && direction.y > 0.0f)
					block.onCollision = true;

				mario->Collision(direction);
			}		

			if(block.onCollision)
			block.Update(deltaTime);
		}

		for (Blocks& block : blocks)
			block.Draw(window);
	
		//mario->draw(window, states);

		window.display();

	}

	return 0;
}*/

#include "Game.h"

int main()
{
	Game game;

	while (game.isRunning())
	{
		game.processInput();
		game.update();
		game.lateUpdate();
		game.draw();	
		game.calculateDeltaTime();
	}
}