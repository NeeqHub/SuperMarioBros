#pragma once
#include "Window.h"
#include "Input.h"
#include "WorkingDirectory.h"
#include "ResourceManager.h"
#include "SceneStateMachine.h"
#include "SceneSplashScreen.h"
#include "SceneGame.h"

class Game
{
public:
	Game();

	void update();
	void lateUpdate();
	void draw();	
	void calculateDeltaTime();
	void processInput();

	bool isRunning() const;

private:
	Window window;
	WorkingDirectory workingDir;
	SceneStateMachine sceneStateMachine;
	ResourceManager<sf::Texture> textureAllocator;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Clock clock;
	float deltaTime;
	
};

