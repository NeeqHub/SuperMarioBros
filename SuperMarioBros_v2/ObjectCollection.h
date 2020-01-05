#pragma once
#include <memory>
#include <vector>
#include "Object.h"
#include "Component.h"

class ObjectCollection
{
public:
	ObjectCollection();
	void add(std::shared_ptr<Object> object);
	void add(std::vector<std::shared_ptr<Object>> objects);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	void processNewObjects();
	void processRemovals();

private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> newObjects;
	SDrawable drawables;
	S_Collidable collidables;

};

