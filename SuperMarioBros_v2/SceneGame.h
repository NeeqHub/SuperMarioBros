#pragma once
#include "Scene.h"
#include "Input.h"
#include "WorkingDirectory.h"
#include "Object.h"
#include "Component.h"
#include "ObjectCollection.h"
#include "ResourceManager.h"
#include "Tile.h"
#include "CSprite.h"
#include "CKeyboardMovement.h"
#include "CAnimation.h"

class SceneGame : public Scene
{
public:
	SceneGame(WorkingDirectory& workingDir, ResourceManager<sf::Texture>& textureAllocator, Window& window);

	void onCreate() override;
	void onDestroy() override;

	void processInput() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window) override;
	void setSwitchToScene(unsigned int id);
	unsigned int switchToState;

	void CreatePlayer();
	void CreateEnemy();

private:
	SharedContext context;
	WorkingDirectory& workingDir;
	Input input;
	ObjectCollection objects;
	ResourceManager<sf::Texture>& textureAllocator;
	STile tileSystem;
	Window& window;

};

