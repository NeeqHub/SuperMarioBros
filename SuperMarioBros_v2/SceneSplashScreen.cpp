#include "SceneSplashScreen.h"

SceneSplashScreen::SceneSplashScreen(WorkingDirectory& workingDir, SceneStateMachine& sceneStateMachine, Window& window, ResourceManager<sf::Texture>& textureAllocator)
	: sceneStateMachine(sceneStateMachine), workingDir(workingDir), window(window), switchToState(0), currentSeconds(0.0f), showForSeconds(3.0f), textureAllocator(textureAllocator) {}

void SceneSplashScreen::onCreate()
{
	int textureID = textureAllocator.add(workingDir.Get()
		+ "mario.png"); //1

		if (textureID >= 0)
		{
			std::shared_ptr<sf::Texture> texture = textureAllocator.get(textureID);
			splashSprite.setTexture(*texture);

			sf::FloatRect spriteSize = splashSprite.getLocalBounds();
			splashSprite.setOrigin(spriteSize.width * 0.5f,
				spriteSize.height * 0.5f);
			splashSprite.setScale(0.5f, 0.5f);

			sf::Vector2u windowCentre = sf::Vector2u(0, 0);
			splashSprite.setPosition(windowCentre.x, windowCentre.y);
		}
}

void SceneSplashScreen::onActivate()
{
	// Resets the currentSeconds count whenever the scene is activated.
	currentSeconds = 0.0f;
}

void SceneSplashScreen::onDestroy() 
{ 
}

void SceneSplashScreen::setSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
}

void SceneSplashScreen::Update(float deltaTime)
{
	currentSeconds += deltaTime;

	if (currentSeconds >= showForSeconds)
	{
		// Switches states.
		sceneStateMachine.switchTo(switchToState);
	}
}

void SceneSplashScreen::Draw(Window& window)
{
	window.draw(splashSprite);
}
