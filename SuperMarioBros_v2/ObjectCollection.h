#pragma once
#include <memory>
#include <vector>
#include "Object.h"

class ObjectCollection
{
public:
	ObjectCollection();
	void add(std::shared_ptr<Object> object);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	void processNewObjects();

private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> newObjects;

};

