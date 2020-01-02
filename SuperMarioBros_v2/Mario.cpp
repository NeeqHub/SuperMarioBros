#include "Mario.h"

Mario::Mario(sf::Texture& texture, sf::Vector2u imageCount, float speed, float jumpHeight, float switchAnimationTime) : animation(&texture, imageCount, switchAnimationTime)
{
	this->speed = speed;
	this->jumpHeight = jumpHeight;
	
	faceRight = true;

	row = 0;

	body.setSize(sf::Vector2f(48.0f, 48.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(sf::Vector2f(0.0f, 0.0f));
	body.setTexture(&texture);

	transform = addComponent<CTransform>();
}

Mario::~Mario()
{
}

void Mario::Collision(sf::Vector2f direction)
{
	if (direction.x < 0.0f)
	{
		// Collision on the left
		velocity.x = 0.0f;
	}
	else if (direction.x < 0.0f)
	{
		// Collision on the right
		velocity.x = 0.0f;
	}
	if (direction.y < 0.0f)
	{
		// Collision on the bottom
		velocity.y = 0.0f;
		canJump = true;
	}
	else if (direction.y > 0.0f)
	{
		// Collision on the top
		velocity.y = 0.0f;
	}

}

void Mario::Update(float deltaTime)
{
	/*velocity.x = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity.x -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity.x += speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		if (canJump)
			velocity.y = -sqrtf(75.0f * 981.0f * jumpHeight);
	}

	velocity.y += 1800.0f * deltaTime;*/

	transform->move(deltaTime);

	// row 0 - idle 
	// row 1 - walk
	// row 2 - jump

	if (velocity.x == 0.0f)
	{
		row = 0;
	}
	else
	{
		row = 1;

		if (velocity.x > 0.0f)
			faceRight = true;
		else
			faceRight = false;

	}

	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime);	

}
/*
void Mario::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body);
	
}*/
