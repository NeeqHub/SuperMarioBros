#include "SceneStateMachine.h"

SceneStateMachine::SceneStateMachine() : scenes(NULL), curScene(NULL)
{
}

void SceneStateMachine::processInput()
{
	if (curScene)
	{
		curScene->processInput();
	}
}

void SceneStateMachine::update(float deltaTime)
{
	if (curScene)
	{
		curScene->Update(deltaTime);
	}
}

void SceneStateMachine::lateUpdate(float deltaTime)
{
	if (curScene)
	{
		curScene->LateUpdate(deltaTime);
	}
}

void SceneStateMachine::draw(Window & window)
{
	if (curScene)
	{
		curScene->Draw(window);
	}
}

unsigned int SceneStateMachine::add(std::shared_ptr<Scene> scene)
{
	auto inserted = scenes.insert(std::make_pair(insertedSceneID, scene));
	insertedSceneID++;
	inserted.first->second->onCreate();

	return insertedSceneID - 1;
}

void SceneStateMachine::switchTo(unsigned int id)
{
	auto it = scenes.find(id);

	if (it != scenes.end())
	{
		if (curScene)
			curScene->onDeactivate();

		curScene = it->second;
		curScene->onActivate();
	}
}

void SceneStateMachine::remove(unsigned int id)
{
	auto it = scenes.find(id);
	
	if (it != scenes.end())
	{
		if (curScene == it->second)
			curScene = nullptr;

		it->second->onDestroy();
		scenes.erase(it);
	}
}

