#pragma once
#include "Component.h"
#include "Collision.h"

class Object;
class CAnimation;

class CCollider : public Component
{
public:
	CCollider(Object* owner);
	~CCollider();

	virtual Manifold Intersects(std::shared_ptr<CCollider> other) = 0;
	virtual void ResolveOverlap(const Manifold& m) = 0;

	CollisionLayer GetLayer() const;
	void SetLayer(CollisionLayer layer);

private:
	CollisionLayer layer;
};

enum Tag
{
	Defult,
	Player,
	Enemy,
	EnemyTurtle,
	Brick,
	Surprise,
	Coin
};

class CBoxCollider : public CCollider
{
public:
	CBoxCollider(Object* owner);

	Manifold Intersects(std::shared_ptr<CCollider> other) override;
	void ResolveOverlap(const Manifold& m) override;

	void SetCollidable(const sf::FloatRect& rect);
	const sf::FloatRect& GetCollidable();

	void SetOffset(const sf::Vector2f& offset);
	void SetOffset(float x, float y);

	void SetSize(const sf::Vector2f& size);
	void SetSize(float width, float height);

	void SetTag(Tag tag);
	Tag GetTag();
	std::shared_ptr<CAnimation> animation;
private:
	void SetPosition();

	Tag tag;
	sf::FloatRect AABB;
	sf::Vector2f offset;
};