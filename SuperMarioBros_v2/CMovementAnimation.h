#pragma once
#include "Component.h"
#include "Object.h"
#include "CAnimation.h"
#include "CVelocity.h"

class Mario;

class CMovementAnimation : public Component
{
public:
	CMovementAnimation(Object* owner);

	void Awake() override;

	void Update(float deltaTime) override;
	void SetMarioState(Mario* mario);

private:
	std::shared_ptr<CVelocity> velocity;
	std::shared_ptr<CAnimation> animation;
	Mario* myMario;
};