#pragma once
#include "Object.h"
#include "CSprite.h"
#include "Animation.h"
#include "Component.h"
#include "CVelocity.h"

class Turtle : public Object
{
public:

	Turtle(SharedContext& context);
	void OnCreate();
	void SetPosition(float x, float y);

private:

	SharedContext& context;
	int turtleTextureID;
	const int frameWidth = 16;
	const int frameHeight = 24;
};
