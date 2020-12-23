#pragma once
#include "Component.h"

class CCamera : public Component
{
public:
	CCamera(Object* owner);

	void LateUpdate(float deltaTime) override;

	void SetWindow(Window* gameWindow);

private:
	Window* window;
};