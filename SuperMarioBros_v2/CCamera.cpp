#include "CCamera.h"
#include "Object.h"

CCamera::CCamera(Object* owner) : Component(owner) { }

void CCamera::LateUpdate(float deltaTime)
{
	if (window)
	{
		sf::View view = window->GetView();

		const sf::Vector2f& targetPos = owner->transform->getPosition();
		//TODO: remove hard-coding of y value
		view.setCenter(targetPos.x, 500);

		window->SetView(view);
	}
}

void CCamera::SetWindow(Window* gameWindow)
{
	window = gameWindow;
}