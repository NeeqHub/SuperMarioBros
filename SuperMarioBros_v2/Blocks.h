#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace std;
using namespace sf;

class Blocks
{
public:
	Blocks(Texture& texture, Vector2u tileSize, unsigned int width, unsigned int height,Vector2f startPosition, int tileId);
	~Blocks();
	RectangleShape shape;
	Texture texture;
	//Collider GetCollider() { return Collider(shape); }
	Collider GetCollider();
	void Draw(RenderWindow& window);
	void ChangeTexture();
	void PlayAnim();
	void Update(float deltaTime);
	float totalTime;
	string tag;
	IntRect uvRect;
	bool isDead;
	bool isUsed;
	bool onCollision;
private:
	unsigned char tileId;
	
	
	
};

