#pragma once
#include <SFML/Graphics.hpp>
#include "SceneStateMachine.h"
#include "WorkingDirectory.h"
#include "ResourceManager.h"

class SceneGameOver : public Scene
{
public:
	SceneGameOver(WorkingDirectory& workingDir, SceneStateMachine& sceneStateMachine, Window& window, ResourceManager<sf::Texture>& textureAllocator);
	~SceneGameOver();

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void setSwitchToScene(unsigned int id);

	void Update(float deltaTime) override;
	void Draw(Window& window) override;

private:
	sf::Texture splashTexture;
	sf::Sprite splashSprite;

	WorkingDirectory& workingDir;
	SceneStateMachine& sceneStateMachine;
	Window& window;
	ResourceManager<sf::Texture>& textureAllocator;

	// We want to show this scene for a set amount of time
	float showForSeconds;

	// How long the scene has currently been visible.
	float currentSeconds;

	// The state we want to transition to when this scenes time expires.
	unsigned int switchToState;
};

