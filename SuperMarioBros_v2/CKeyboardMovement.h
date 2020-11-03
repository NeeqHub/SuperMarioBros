#pragma once
#include "Component.h"

class Object;

class CKeyboardMovement : public Component
{
public:
	CKeyboardMovement(Object* owner);

	void Awake() override;
	void setMovementSpeed(float moveSpeed);

	void Update(float deltaTIme) override;

private:
	float moveSpeed;
	std::shared_ptr<CAnimation> animation;
	std::shared_ptr<C_Velocity> velocity;
	SceneStateMachine sceneStateMachine;

	const float GRAVITY = 9.81f;
	const float MAX_VELOCITY = -10.0f;
	const float MAX_AIR_TIME = 1.2f;

	float timeInAir = 0.0f;
	float jumpImpulseTime = 2.0f;
	float jumpImpulseVel = -10.0f;
	float jumpAccel = -500.0f;

	float totalTimeInAir;
	float currentTime;
};