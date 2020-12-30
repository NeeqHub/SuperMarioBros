#pragma once
#include <SFML/Graphics.hpp>

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
	IdleBig,
	Walk,
	WalkBig,
	Jump,
	JumpBig,
	Death,
	Projectile
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


