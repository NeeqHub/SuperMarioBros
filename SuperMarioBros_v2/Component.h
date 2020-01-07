#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Input.h"
#include "ResourceManager.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Object;

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

class CTransform : public Component
{
public:
	CTransform(Object* owner);
	~CTransform() {}

	float speed = 100.0f;
	bool faceRight = true;
	bool canJump = true;
	float jumpHeight = 50.0f;

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);

	void addPosition(float x, float y);
	void addPosition(sf::Vector2f pos);

	void setX(float x);
	void setY(float y);

	void addX(float x);
	void addY(float y);

	const sf::Vector2f& getPosition() const;
	void SetStatic(bool isStatic);
	bool isStatic() const;

private:
	sf::Vector2f position;
	sf::Vector2f velocity;
	bool isStaticTransform;
};

namespace tex
{
	enum ID { Mario, Blocks, Enemy, Powerups };
}

struct FrameData
{
	int id;
	int x;
	int y;
	int height;
	int width;
	float displayTimeSeconds;
};

enum class AnimationState
{
	None,
	Idle,
	Walk
};

enum class FaceDirection
{
	None,
	Right,
	Left
};

class Animation
{
public:
	Animation(FaceDirection direction);

	void AddFrame(int id, int x, int y, int height, int width, float frameTime);
	const FrameData* GetCurrentFrame() const;
	bool UpdateFrame(float deltaTime);
	void Reset();

	void SetDirection(FaceDirection direction);
	FaceDirection GetDirection() const;

private:
	void IncrementFrame();
	std::vector<FrameData> frames;
	int currentFrameIndex;
	float currentFrameTime;
	FaceDirection direction;

};

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

class CSprite : public Component, public CDrawable
{
public:
	CSprite(Object* owner);
	void Load(const std::string& filePath);
	void Load(int id);
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window);
	void SetTextureAllocator(ResourceManager<sf::Texture>* allocator); // 1
	void SetTextureRect(int x, int y, int width, int height);
	void SetTextureRect(const sf::IntRect& rect);
	void SetScale(unsigned int x, unsigned int y);

private:
	ResourceManager<sf::Texture>* allocator;
	//sf::Texture texture;
	sf::Sprite sprite;
	int currentTextureID;
};

class CAnimation : public Component
{
public:

	CAnimation(Object* owner);

	void Awake() override;
	void Update(float deltaTime);
	void AddAnimation(AnimationState state, std::shared_ptr<Animation> animation);
	void SetAnimationState(AnimationState state);
	void SetAnimationDirection(FaceDirection direction);
	const AnimationState& GetAnimationState() const;

private:
	std::shared_ptr<CSprite> sprite;
	std::map<AnimationState, std::shared_ptr<Animation>> animations;
	std::pair<AnimationState, std::shared_ptr<Animation>> currentAnimation;
};

class CKeyboardMovement : public Component
{
public:
	CKeyboardMovement(Object* owner);

	void Awake() override;
	void setInput(Input* input);
	void setMovementSpeed(int moveSpeed);

	void Update(float deltaTIme) override;

private:
	int moveSpeed;
	Input* input;
	std::shared_ptr<CAnimation> animation;
};

enum class CollisionLayer
{
	Default = 1,
	Player = 2,
	Tile = 3
};

struct Manifold
{
	bool colliding = false;
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

class CBoxCollider : public CCollider
{
public:
	CBoxCollider(Object* owner);

	Manifold Intersects(std::shared_ptr<CCollider> other) override;
	void ResolveOverlap(const Manifold& m) override;

	void SetCollidable(const sf::FloatRect& rect);
	const sf::FloatRect& GetCollidable();

private:
	void SetPosition();

	sf::FloatRect AABB;
	sf::Vector2f offset;
};

class C_InstanceID : public Component
{
public:
	C_InstanceID(Object* owner);
	~C_InstanceID();

	unsigned int Get() const;

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

class C_Camera : public Component
{
public:
	C_Camera(Object* owner);

	void LateUpdate(float deltaTime) override;

	void SetWindow(Window* gameWindow);

private:
	Window* window;
};

class C_Collidable
{
public:
	virtual void OnCollisionEnter(std::shared_ptr<CBoxCollider> other) {};
	virtual void OnCollisionStay(std::shared_ptr<CBoxCollider> other) {};
	virtual void OnCollisionExit(std::shared_ptr<CBoxCollider> other) {};
};

class C_RemoveObjectOnCollisionEnter : public Component, public C_Collidable
{
public:
	C_RemoveObjectOnCollisionEnter(Object* owner);

	void OnCollisionEnter(std::shared_ptr<CBoxCollider> other) override;
};






