#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"

class Mario : public sf::Drawable
{
public:
	Mario(sf::Texture& texture, sf::Vector2u imageCount, float speed, float jumpHeight, float switchAnimationTime);
	~Mario();

	void Collision(sf::Vector2f direction);
	void Update(float deltaTime);

	sf::Vector2f getPosition() { return body.getPosition(); }
	Collider getCollider() { return Collider(body); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape body;
	sf::Vector2f velocity;

	unsigned int row;
	float speed;
	float jumpHeight;
	bool faceRight;
	bool canJump;

	Animation animation;
};

