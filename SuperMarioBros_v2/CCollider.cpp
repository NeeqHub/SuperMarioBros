#include "CCollider.h"
#include "Object.h"

CCollider::CCollider(Object* owner)
	: Component(owner), layer(CollisionLayer::Default) {}

CCollider::~CCollider() {}

CollisionLayer CCollider::GetLayer() const
{
	return layer;
}

void CCollider::SetLayer(CollisionLayer layer)
{
	this->layer = layer;
}

CBoxCollider::CBoxCollider(Object* owner) : CCollider(owner),
offset(sf::Vector2f(0.0f, 0.0f)), animation(owner->getComponent<CAnimation>()) {}

void CBoxCollider::SetCollidable(const sf::FloatRect& rect)
{
	AABB = rect;
	SetPosition();
}

const sf::FloatRect& CBoxCollider::GetCollidable()
{
	SetPosition();
	return AABB;
}

void CBoxCollider::SetPosition()
{
	const sf::Vector2f& pos = owner->transform->getPosition();

	AABB.left = pos.x - (AABB.width / 2) + offset.x;
	AABB.top = pos.y - (AABB.height / 2) + offset.y;
}

Manifold CBoxCollider::Intersects(std::shared_ptr<CCollider> other)
{
	Manifold m;
	m.colliding = false; // We default to not colliding.

	std::shared_ptr<CBoxCollider> boxCollider = std::dynamic_pointer_cast<CBoxCollider>(other); // 1

	if (boxCollider) // 2
	{
		const sf::FloatRect& rect1 = GetCollidable();
		const sf::FloatRect& rect2 = boxCollider->GetCollidable();

		if (rect1.intersects(rect2)) // 3
		{
			float xDiff = (rect1.left + (rect1.width * 0.5f)) - (rect2.left + (rect2.width * 0.5f)); // 3
			float yDiff = (rect1.top + (rect1.height * 0.5f)) - (rect2.top + (rect2.height * 0.5f));

			if (fabs(xDiff) > fabs(yDiff)) // 4
			{
				if (xDiff > 0)
					m.collisionDirection = CollisionDirection::Left;
				else
					m.collisionDirection = CollisionDirection::Right;
			}
			else
			{
				if (yDiff > 0)
					m.collisionDirection = CollisionDirection::Bottom;
				else
					m.collisionDirection = CollisionDirection::Top;
			}

			m.colliding = true; // 4
			m.other = &rect2;
		}
	}

	return m;
}

void CBoxCollider::ResolveOverlap(const Manifold& m)
{
	auto transform = owner->transform;

	if (transform->isStatic()) { return; } // 1

	const sf::FloatRect& rect1 = GetCollidable();
	const sf::FloatRect* rect2 = m.other;

	float resolve = 0; // 2

	float xDiff = (rect1.left + (rect1.width * 0.5f)) - (rect2->left + (rect2->width * 0.5f)); // 3
	float yDiff = (rect1.top + (rect1.height * 0.5f)) - (rect2->top + (rect2->height * 0.5f));

	if (fabs(xDiff) > fabs(yDiff)) // 4
	{
		if (xDiff > 0) // Colliding on the left.
		{
			// We add a positive x value to move the object to the right.
			resolve = (rect2->left + rect2->width) - rect1.left;
		}
		else // Colliding on the right.
		{
			// We add a negative x value to move the object to the left.
			// hotfix, push player 10 units back, to change later
			resolve = -((rect1.left + rect1.width) - rect2->left+10);
		}

		transform->addPosition(resolve, 0); // 5
	}
	else
	{
		if (yDiff > 0) // Colliding above.
		{
			// We add a positive y value to move the object down.
			resolve = (rect2->top + rect2->height) - rect1.top;
		}
		else // Colliding below
		{
			// We add a negative y value to move the object up.
			resolve = -((rect1.top + rect1.height) - rect2->top);
		}

		transform->addPosition(0, resolve); // 5
	}
}

void CBoxCollider::SetOffset(const sf::Vector2f& offset)
{
	this->offset = offset;
}

void CBoxCollider::SetOffset(float x, float y)
{
	offset.x = x;
	offset.y = y;
}

void CBoxCollider::SetSize(const sf::Vector2f& size)
{
	AABB.width = size.x;
	AABB.height = size.y;
}

void CBoxCollider::SetSize(float width, float height)
{
	AABB.width = width;
	AABB.height = height;
}

void CBoxCollider::SetTag(Tag tag)
{
	this->tag = tag;
}

Tag CBoxCollider::GetTag()
{
	return tag;
}