#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Input.h"
#include "ResourceManager.h"
#include <vector>

class Object;

class Component
{
public:
	Component(Object* owner) : owner(owner) {}
	~Component() {}

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};
	virtual void Draw(Window& window) {};

protected:
	Object* owner;
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

private:
	sf::Vector2f position;
	sf::Vector2f velocity;
};

namespace tex
{
	enum ID { Mario, Blocks, Enemy, Powerups };
}

/*class CGraphics : public Component
{
public:
	CGraphics(Object* owner) : Component(owner) {}
	~CGraphics() {}

	void LateUpdate(float deltaTime) override;

	void setTexture(const std::string& path);
	void Draw(Window& window) override;

private:
	sf::Texture texture;
	sf::RectangleShape shape;

};*/

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

class CSprite : public Component
{
public:
	CSprite(Object* owner);
	void Load(const std::string& filePath);
	void Load(int id);
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window) override;
	void SetTextureAllocator(ResourceManager<sf::Texture>* allocator); // 1
	void SetTextureRect(int x, int y, int width, int height);
	void SetTextureRect(const sf::IntRect& rect);

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

