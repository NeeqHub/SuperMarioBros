#pragma once

#include "Component.h"
#include <memory>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void awake();
	void start();
	void update(float deltaTime);
	void lateUpdate(float deltaTime);
	void draw(sf::RenderWindow window);

	template <typename T>
	std::shared_ptr<T> addComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		for (auto& exisitingComponent : components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				return std::dynamic_pointer_cast<T>(exisitingComponent);
			}
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(this);
		components.push_back(newComponent);

		return newComponent;
	}

	template <typename T>
	std::shared_ptr<T> getComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		for (auto& exisitingComponent : components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				return std::dynamic_pointer_cast<T>(exisitingComponent);
			}
		}

		return nullptr;
	}

	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<CTransform> transform;
};

