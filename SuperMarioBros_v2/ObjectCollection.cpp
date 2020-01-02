#include "ObjectCollection.h"

ObjectCollection::ObjectCollection()
{
}

void ObjectCollection::Update(float deltaTime)
{
	for (auto& obj : objects)
	{
		obj->Update(deltaTime);
	}
}

void ObjectCollection::LateUpdate(float deltaTime)
{
	for (auto& obj : objects)
	{
		obj->LateUpdate(deltaTime);
	}
}

void ObjectCollection::Draw(Window& window)
{
	for (auto& obj : objects)
	{
		obj->Draw(window);
	}
}

void ObjectCollection::add(std::shared_ptr<Object> obj)
{
	newObjects.push_back(obj);
}

void ObjectCollection::processNewObjects()
{
	if (newObjects.size() > 0)
	{
		for (const auto& obj : newObjects)
		{
			obj->Awake();
		}

		for (const auto& obj : newObjects)
		{
			obj->Start();
		}

		objects.assign(newObjects.begin(), newObjects.end());
		newObjects.clear();
	}
}