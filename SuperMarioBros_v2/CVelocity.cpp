#include "CVelocity.h"

CVelocity::CVelocity(Object* owner) :
	Component(owner), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), maxVelocity(8.f, 8.f), currentTime(0.0f) { }

void CVelocity::Update(float deltaTime)
{
	if (owner->getComponent<C_InstanceID>()->Get() == 478)
		velocity.x *= 0.975f;

	velocity.x += acceleration.x * deltaTime;

	velocity.y += acceleration.y * deltaTime;

	ClampVelocity();

	owner->transform->addPosition(velocity);
}

void CVelocity::Set(const sf::Vector2f& vel)
{
	velocity = vel;

	//ClampVelocity();
}

void CVelocity::SetAcc(float x, float y)
{
	acceleration.x = x;
	acceleration.y = y;
}

void CVelocity::Set(float x, float y)
{
	velocity.x = x;
	velocity.y = y;

	//ClampVelocity();
}

const sf::Vector2f& CVelocity::Get() const
{
	return velocity;
}

const float& CVelocity::GetX() const
{
	return velocity.x;
}

const float& CVelocity::GetY() const
{
	return velocity.y;
}

void CVelocity::ClampVelocity()
{
	if (fabs(velocity.x) > maxVelocity.x)
	{
		if (velocity.x > 0.f)
		{
			velocity.x = maxVelocity.x;
		}
		else
		{
			velocity.x = -maxVelocity.x;
		}
	}

	if (fabs(velocity.y) > maxVelocity.y)
	{
		if (velocity.y > 0.f)
		{
			velocity.y = maxVelocity.y;
		}
		else
		{
			velocity.y = -maxVelocity.y;
		}
	}
}