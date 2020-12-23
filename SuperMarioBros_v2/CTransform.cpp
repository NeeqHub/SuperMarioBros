#include "CTransform.h"

CTransform::CTransform(Object* owner) : Component(owner), position(0.0f, 0.0f), isStaticTransform(false)
{
}

void CTransform::SetStatic(bool isStatic) { isStaticTransform = isStatic; }

bool CTransform::isStatic() const { return isStaticTransform; }

void CTransform::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void CTransform::setPosition(sf::Vector2f pos)
{
	position = pos;
}

void CTransform::addPosition(float x, float y)
{
	position.x += x;
	position.y += y;
}

void CTransform::addPosition(sf::Vector2f pos)
{
	position += pos;
}

void CTransform::setX(float x)
{
	position.x = x;
}

void CTransform::setY(float y)
{
	position.y = y;
}

void CTransform::addX(float x)
{
	position.x += x;
}

void CTransform::addY(float y)
{
	position.y += y;
}

const sf::Vector2f& CTransform::getPosition() const
{
	return position;
}
