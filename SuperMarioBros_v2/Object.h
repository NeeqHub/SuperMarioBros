#pragma once
#include <vector>
#include "Window.h"
#include "Component.h"

class Object
{
public:
	Object();
	
	void Awake();
	void Start();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

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

		// if component is drawable
		if (std::dynamic_pointer_cast<CDrawable>(newComponent))
		{
			drawable = std::dynamic_pointer_cast<CDrawable>(newComponent);
		}

		return newComponent;
	}

	template <typename T>
	std::shared_ptr<T> getComponent()
	{
		//static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		for (auto& exisitingComponent : components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				return std::dynamic_pointer_cast<T>(exisitingComponent);
			}
		}

		return nullptr;
	}

	bool IsQueuedForRemoval();
	void QueueForRemoval();

	std::shared_ptr<CTransform> transform;
	std::shared_ptr<CDrawable> GetDrawable();
	std::shared_ptr<C_InstanceID> instanceID;

private:
	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<CDrawable> drawable;
	bool queuedForRemoval;
	
};

