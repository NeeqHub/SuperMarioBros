#include "Blocks.h"
#include <iostream>

Blocks::Blocks (Texture& texture, Vector2u tileSize, unsigned int width, unsigned int height, Vector2f position, int tileId)
{
	this->texture = texture;
	this->tileId = tileId;
	shape.setSize(Vector2f(48.0f, 48.0f));
	shape.setOrigin(shape.getSize() / 2.0f);
	shape.setPosition(position);
	shape.setTexture(&texture);
	isDead = false;
	isUsed = false;
	onCollision = false;

	totalTime = 0.0f;

	// 0 - ground
	// 1 - brick
	// 2 - questionBlock
	// 3 - deadQuestionBlock
	// 4 - block

	switch (this->tileId)
	{
	case 0: uvRect.top = 0.0f;
			uvRect.left = 0.0f;
			tag = "ground";
			break;

	case 1: uvRect.top = 0.0f;
			uvRect.left = 16.0f; 
			tag = "brick";
			break;

	case 2: uvRect.top = 0.0f;
			uvRect.left = 32.0f;
			tag = "questionBlock";
			break;

	case 3: uvRect.top = 0.0f;
			uvRect.left = 48.0f; 
			tag = "deadQuestionBlock";
			break;
	}

	uvRect.height = 16.0f;
	uvRect.width = 16.0f;

	shape.setTextureRect(uvRect);
}

Collider Blocks::GetCollider()
{
	return Collider(shape);
}

void Blocks::ChangeTexture()
{
	
}

void Blocks::Update(float deltaTime)
{	
	totalTime += deltaTime;

	if (!isUsed)
	{	
		shape.move(Vector2f(0.0f, -15.0f));
		isUsed = true;
	}

	if (totalTime > 0.15f && isUsed && !isDead)
	{
		shape.move(Vector2f(0.0f, 15.0f));

		uvRect.top = 0.0f;
		uvRect.left = 48.0f;
		shape.setTextureRect(uvRect);
		isDead = true;
	}
}

void Blocks::PlayAnim()
{

}

Blocks::~Blocks()
{
}

void Blocks::Draw(RenderWindow& window)
{
	window.draw(shape);
}


