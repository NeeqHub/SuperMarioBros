#include "SceneGameOver.h"

SceneGameOver::SceneGameOver(WorkingDirectory& workingDir, SceneStateMachine& sceneStateMachine, Window& window, ResourceManager<sf::Texture>& textureAllocator)
	: sceneStateMachine(sceneStateMachine), workingDir(workingDir), window(window), switchToState(0), currentSeconds(0.0f), showForSeconds(3.0f), textureAllocator(textureAllocator) {}


SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::onCreate()
{
	int textureID = textureAllocator.add(workingDir.Get()
		+ "gameoverScreen.png"); //1

	if (textureID >= 0)
	{
		std::shared_ptr<sf::Texture> texture = textureAllocator.get(textureID);
		splashSprite.setTexture(*texture);

		sf::FloatRect spriteSize = splashSprite.getLocalBounds();
		splashSprite.setOrigin(spriteSize.width * 0.5f, spriteSize.height * 0.5f);
		splashSprite.setScale(5.0f, 5.0f);

		sf::Vector2u windowCentre = sf::Vector2u(960, 540);
		splashSprite.setPosition(windowCentre.x, windowCentre.y);
	}
}

void SceneGameOver::onActivate()
{
	// Resets the currentSeconds count whenever the scene is activated.
	currentSeconds = 0.0f;
}

void SceneGameOver::onDestroy()
{
}

void SceneGameOver::setSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	//switchToState = id;
}

void SceneGameOver::Update(float deltaTime)
{
	currentSeconds += deltaTime;

	if (currentSeconds >= showForSeconds)
	{
		// Switches states.
		//sceneStateMachine.switchTo(switchToState);
		//close app
		std::exit(0);
	}
}

void SceneGameOver::Draw(Window& window)
{
	window.draw(splashSprite);
}

