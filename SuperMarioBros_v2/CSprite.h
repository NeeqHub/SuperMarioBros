#pragma once
#include "Component.h"
#include "CTransform.h"
#include "CDrawable.h"

class CSprite : public Component, public CDrawable
{
public:
	CSprite(Object* owner);
	void Load(const std::string& filePath);
	void Load(int id);
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window);
	void SetTextureRect(int x, int y, int width, int height);
	void SetTextureRect(const sf::IntRect& rect);
	void SetScale(unsigned int x, unsigned int y);

private:
	//ResourceManager<sf::Texture>* allocator;
	sf::Sprite sprite;
	int currentTextureID;
};