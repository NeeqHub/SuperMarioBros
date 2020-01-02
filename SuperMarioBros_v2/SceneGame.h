#pragma once
#include "Scene.h"
#include "Input.h"
#include "WorkingDirectory.h"
#include "Object.h"
#include "Component.h"
#include "ObjectCollection.h"
#include "ResourceManager.h"

class SceneGame : public Scene
{
public:
	SceneGame(WorkingDirectory& workingDir, ResourceManager<sf::Texture>& textureAllocator);

	void onCreate() override;
	void onDestroy() override;

	void processInput() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window) override;

private:
	//std::shared_ptr<Object> mario;
	WorkingDirectory& workingDir;
	Input input;
	ObjectCollection objects;
	ResourceManager<sf::Texture>& textureAllocator;
	
};

