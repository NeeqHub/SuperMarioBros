#include "CSprite.h"
#include "Object.h"

CSprite::CSprite(Object* owner) : Component(owner), currentTextureID(-1) // Set current texture id to -1 
{}

/*void CSprite::SetTextureAllocator(ResourceManager<sf::Texture>* allocator)
{
	this->allocator = allocator;
}*/

void CSprite::Load(int id)
{
	// Check its not already our current texture.
	if (id >= 0 && id != currentTextureID)
	{
		currentTextureID = id;
		std::shared_ptr<sf::Texture> texture = owner->context->textureAllocator->get(id);
		sprite.setTexture(*texture);
	}
}

void CSprite::Load(const std::string& filePath)
{
	int textureID = owner->context->textureAllocator->add(filePath);

	if (textureID >= 0 && textureID != currentTextureID)
	{
		currentTextureID = textureID;

		// Also retrieve texture allocator from shared context.
		std::shared_ptr<sf::Texture> texture = owner->context->textureAllocator->get(textureID);
		sprite.setTexture(*texture);
	}
}

void CSprite::Draw(Window& window)
{
	window.draw(sprite);
}

void CSprite::LateUpdate(float deltaTime)
{
	//sprite.setPosition(owner->transform->getPosition());
	sf::Vector2f pos = owner->transform->getPosition();
	const sf::IntRect& spriteBounds = sprite.getTextureRect();
	const sf::Vector2f& spriteScale = sprite.getScale();
	sprite.setPosition(
		pos.x - ((abs(spriteBounds.width) * 0.5f) * spriteScale.x),
		pos.y - ((abs(spriteBounds.height) * 0.5f) * spriteScale.y)
	);
}



void CSprite::SetTextureRect(int x, int y, int width, int height)
{
	sprite.setTextureRect(sf::IntRect(x, y, width, height));
}

void CSprite::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

void CSprite::SetScale(unsigned int x, unsigned int y)
{
	sprite.setScale(x, y);
}