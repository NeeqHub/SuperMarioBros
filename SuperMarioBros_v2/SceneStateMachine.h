#pragma once
#include "Scene.h"
#include "Window.h"
#include <memory>
#include <unordered_map>

class SceneStateMachine
{
public:
	SceneStateMachine();
	
	void processInput();
	void update(float deltaTime);
	void lateUpdate(float deltaTime);
	void draw(Window& window);

	unsigned int add(std::shared_ptr<Scene> scene);
	void switchTo(unsigned int id);
	void remove(unsigned int id);

private:
	std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
	std::shared_ptr<Scene> curScene;

	unsigned int insertedSceneID;
};

