#include "Game.h"

Game::Game() : window("SuperMarioBros")
{
	std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(workingDir, sceneStateMachine, window, textureAllocator);
	std::shared_ptr<SceneGame> gameScene = std::make_shared<SceneGame>(workingDir, textureAllocator, window);

	unsigned int splashScreenID = sceneStateMachine.add(splashScreen);
	unsigned int gameSceneID = sceneStateMachine.add(gameScene);

	splashScreen->setSwitchToScene(gameSceneID);

	sceneStateMachine.switchTo(splashScreenID); 

	deltaTime = clock.restart().asSeconds();
}

void Game::processInput()
{
	sceneStateMachine.processInput();
}

void Game::update()
{
	window.update();
	sceneStateMachine.update(deltaTime);
}

void Game::lateUpdate()
{
	sceneStateMachine.lateUpdate(deltaTime);
}

void Game::draw()
{
	window.beginDraw();
	sceneStateMachine.draw(window);
	window.endDraw();
}

bool Game::isRunning() const
{
	return window.isOpen();
}

void Game::calculateDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

