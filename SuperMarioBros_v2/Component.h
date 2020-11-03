#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Input.h"
#include "ResourceManager.h"
#include "WorkingDirectory.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "SceneStateMachine.h"
#include "SFML/Audio.hpp"
#include "Animation.h"

class Object;
class CAnimation;
class CVelocity;

struct EnumClassHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

class Component
{
public:
	Component(Object* owner) : owner(owner) {}
	~Component() {}

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};

	Object* owner;

protected:
	
};

namespace tex
{
	enum ID { Mario, Blocks, Enemy, Powerups };
}

class CDrawable
{
public:
	CDrawable();
	virtual ~CDrawable();

	virtual void Draw(Window& window);
	void SetSortOrder(int order);
	int GetSortOrder() const;

private:
	int sortOrder;
};

class SDrawable
{
public:
	void Add(std::vector<std::shared_ptr<Object>>& object);
	void ProcessRemovals();
	void Draw(Window& window);


private:
	void Add(std::shared_ptr<Object> object);
	void Sort();
	std::vector<std::shared_ptr<Object>> drawables;
};

class CVelocity;



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

class C_InstanceID : public Component
{
public:
	C_InstanceID(Object* owner);
	~C_InstanceID();

	unsigned int Get();

private:
	static unsigned int count;
	unsigned int id;
};

class Quadtree
{
public:
	Quadtree();
	Quadtree(int maxObjects, int maxLevels, int level,
		sf::FloatRect bounds, Quadtree* parent);

	// Inserts object into our quadtree.
	void Insert(std::shared_ptr<CBoxCollider> object);

	// Removes object from our quadtree when we no longer need it to collide.
	void Remove(std::shared_ptr<CBoxCollider> object);

	// Removes all objects from tree.
	void Clear();

	// Returns vector of colliders that intersect with the search area.
	std::vector<std::shared_ptr<CBoxCollider>>
		Search(const sf::FloatRect& area);

	// Returns the bounds of this node.
	const sf::FloatRect& GetBounds() const;

	// To delete
	void DrawDebug();
private:
	void Search(const sf::FloatRect& area,
		std::vector<std::shared_ptr<CBoxCollider>>&
		overlappingObjects);

	// Returns the index for the node that will contain 		
	// the object. -1 is returned if it is this node.
	int GetChildIndexForObject(const sf::FloatRect& objectBounds);

	// Creates the child nodes.
	void Split();

	// We’ll use these as indices in our array of children.
	static const int thisTree = -1;
	static const int childNE = 0;
	static const int childNW = 1;
	static const int childSW = 2;
	static const int childSE = 3;

	int maxObjects;
	int maxLevels;

	// nulptr is this is the base node.
	Quadtree* parent;
	std::shared_ptr<Quadtree> children[4];

	// Stores objects in this node.
	std::vector<std::shared_ptr<CBoxCollider>> objects;

	// How deep the current node is from the base node. 
	// The first node starts at 0 and then its child node 	
	// is at level 1 and so on.
	int level;

	// The bounds of this node.
	sf::FloatRect bounds;
};

struct ComponentPairHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		std::size_t x = t.first->owner->instanceID->Get();
		std::size_t y = t.second->owner->instanceID->Get();

		return (x >= y) ? (x * x + x + y) : (y * y + y + x);
	}
};

class S_Collidable
{
public:
	S_Collidable();

	void Add(std::vector<std::shared_ptr<Object>>& objects);
	void ProcessRemovals();
	void Update();

private:
	void Resolve();
	void ProcessCollidingObjects();
	std::unordered_map<CollisionLayer, Bitmask, EnumClassHash> collisionLayers;
	std::unordered_map<CollisionLayer, std::vector<std::shared_ptr<CBoxCollider>>, EnumClassHash> collidables;
	std::unordered_set<std::pair<std::shared_ptr<CBoxCollider>, std::shared_ptr<CBoxCollider>>, ComponentPairHash> objectsColliding;

	// This is used to store collision layer data i.e. which layers can collide.
	//std::map<CollisionLayer, Bitmask> collisionLayers;

	// The collision system stores all collidables along with their layer.
	//std::map<CollisionLayer, std::vector<std::shared_ptr<CBoxCollider>>> collidables;

	// The quadtree stores the collidables in a spatial aware structure.
	Quadtree collisionTree;
};



class C_Collidable
{
public:
	virtual void OnCollisionEnter(std::shared_ptr<CBoxCollider> other) {};
	virtual void OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m) {};
	virtual void OnCollisionStay(std::shared_ptr<CBoxCollider> other, Manifold m) {};
	virtual void OnCollisionExit(std::shared_ptr<CBoxCollider> other) {};
};

class C_RemoveObjectOnCollisionEnter : public Component, public C_Collidable
{
public:
	C_RemoveObjectOnCollisionEnter(Object* owner);

	void OnCollisionEnter(std::shared_ptr<CBoxCollider> other) override;
};

class ObjectCollection;

struct SharedContext
{
	Input* input;
	ObjectCollection* objects;
	WorkingDirectory* workingDir;
	ResourceManager<sf::Texture>* textureAllocator;
	Window* window;
};

class OutputColliders : public Component, public C_Collidable
{
public:
	OutputColliders(Object* owner);
	void OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m)override;
	void OnCollisionStay(std::shared_ptr<CBoxCollider> other, Manifold m)override;
	void OnCollisionExit(std::shared_ptr<CBoxCollider> other)override;
};

class EnemyMovement : public Component
{
public:
	EnemyMovement(Object* owner);
	void Awake();
	void Update(float deltaTime);

private:
	std::shared_ptr<CVelocity> velocity;
	float enemyMovementSpeed;
	float deathTime;
};

class EnemyTurtleMovement : public Component, public C_Collidable
{
public:
	EnemyTurtleMovement(Object* owner);
	void Awake();
	void Update(float deltaTime);
	void OnCollisionStay(std::shared_ptr<CBoxCollider> other, Manifold m)override;
	void OnCollisionExit(std::shared_ptr<CBoxCollider> other)override;

private:
	std::shared_ptr<CVelocity> velocity;
	float enemyMovementSpeed;
	float deathTime;
};

class EnemyAnim : public Component, public C_Collidable
{
public:
	EnemyAnim(Object* owner);
	void Awake();
	void Update(float deltaTime) override;
	void OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m) override;
private:
	std::shared_ptr<CAnimation> animation;
};

class EnemyTurtleAnim : public Component, public C_Collidable
{
public:
	EnemyTurtleAnim(Object* owner);
	void Awake();
	void Update(float deltaTime) override;
	void OnCollisionEnter(std::shared_ptr<CBoxCollider> other, Manifold m) override;
private:
	std::shared_ptr<CAnimation> animation;
	std::shared_ptr<CVelocity> velocity;
};

class BlocksAnim : public Component
{
public:
	BlocksAnim(Object* owner);
	void Update(float deltaTime) override;
	void OnDestory();
private:
	float currentTime;
	float currentY;
};



