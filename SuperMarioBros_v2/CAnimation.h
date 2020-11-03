#pragma once
#include "Animation.h"
#include "Component.h"
#include "CSprite.h"

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