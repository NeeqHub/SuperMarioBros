#pragma once
#include "SFML/Graphics.hpp"

enum class CollisionLayer
{
	Default = 1,
	Player = 2,
	Tile = 3
};

enum CollisionDirection
{
	Top,
	Bottom,
	Right,
	Left
};

struct Manifold
{
	bool colliding = false;
	CollisionDirection collisionDirection;
	const sf::FloatRect* other;
};