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

class Mario
{

public: 

	Mario(SharedContext& context, ObjectCollection* objects);
	void OnCreate();
	void Promote();
	void Demote();
	MarioState GetMarioState() const;
	

private:

	SharedContext& context;
	ObjectCollection* objects;
	std::shared_ptr<Object> mario;
	MarioState marioState = MarioState::BigMario;
	
};