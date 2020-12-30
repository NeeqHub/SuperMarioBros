#pragma once
#include "Component.h"
#include "Object.h"
#include "CSprite.h"
#include "ObjectCollection.h"
#include "CCamera.h"
#include "CMovementAnimation.h"
#include "CKeyboardMovement.h"

enum class MarioState
{
	SmallMario,
	BigMario,
	FireMario
};

class Mario : public Object
{

public: 

	Mario(SharedContext& context);
	void OnCreate();
	void Promote();
	void Demote();
	MarioState GetMarioState();
	

private:

	SharedContext& context;
	MarioState marioState = MarioState::SmallMario;
	void SetBigMarioCollider();
	
};