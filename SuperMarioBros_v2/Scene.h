#pragma once
#include "Window.h"

class Scene
{
public:
	Scene() {};
	
	virtual void onCreate() {};
	virtual void onDestroy() {};
	virtual void onActivate() {};
	virtual void onDeactivate() {};

	virtual void processInput() {};
	virtual void Update(float deltaTime) {}; 
	virtual void LateUpdate(float deltaTime) {};
	virtual void Draw(Window& window) {};
};

