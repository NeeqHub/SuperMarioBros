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
	drawables.Draw(window);
}

void ObjectCollection::add(std::shared_ptr<Object> obj)
{
	newObjects.push_back(obj);
}

void ObjectCollection::add(std::vector<std::shared_ptr<Object>> objs)
{
	newObjects = objs;
	//newObjects.insert(newObjects.end(), objs.begin(), objs.end());
}

void ObjectCollection::processNewObjects()
{
	if (newObjects.size() > 0)
	{
		for (const auto& o : newObjects)
		{
			o->Awake();
		}

		for (const auto& o : newObjects)
		{
			o->Start();
		}

		objects.insert(objects.end(), newObjects.begin(), newObjects.end());

		drawables.Add(newObjects); // New Line.

		newObjects.clear();
	}
}

void ObjectCollection::processRemovals()
{
	auto objIterator = objects.begin();
	while (objIterator != objects.end())
	{
		auto obj = *objIterator;

		if (obj->IsQueuedForRemoval())
		{
			objIterator = objects.erase(objIterator);
		}
		else
		{
			++objIterator;
		}
	}

	drawables.ProcessRemovals(); // New Line.
}