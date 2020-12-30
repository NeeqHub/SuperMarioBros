#pragma once
#include "Object.h"
#include "CSprite.h"
#include "Animation.h"
#include "Component.h"
#include "CVelocity.h"

class Mushroom : public Object
{
public:

	Mushroom(SharedContext& context);
	void OnCreate();
	void SetPosition(float x, float y);

private:

	SharedContext& context;
	int mushroomTextureID;
	const int frameWidth = 16;
	const int frameHeight = 16;
};