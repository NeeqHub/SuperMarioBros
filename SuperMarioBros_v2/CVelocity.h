#pragma once
#include "Component.h"
#include "Object.h"

class CVelocity : public Component
{
public:
	CVelocity(Object* owner);

	void Update(float deltaTime) override;

	void Set(const sf::Vector2f& vel);
	void Set(float x, float y);
	void SetAcc(float x, float y);

	const sf::Vector2f& Get() const;
	const float& GetX() const;
	const float& GetY() const;

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
private:

	void ClampVelocity();
	sf::Vector2f maxVelocity;
	float currentTime;
};