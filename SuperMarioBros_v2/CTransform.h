#pragma once
#include "Component.h"

class CTransform : public Component
{
public:
	CTransform(Object* owner);
	~CTransform() {}

	float speed = 100.0f;
	bool faceRight = true;
	bool canJump = true;
	float jumpHeight = 50.0f;

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);

	void addPosition(float x, float y);
	void addPosition(sf::Vector2f pos);

	void setX(float x);
	void setY(float y);

	void addX(float x);
	void addY(float y);

	sf::Vector2f velocity;
	const sf::Vector2f& getPosition() const;
	void SetStatic(bool isStatic);
	bool isStatic() const;

private:
	sf::Vector2f position;

	bool isStaticTransform;
};