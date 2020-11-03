#include "CAnimation.h"
#include "Object.h"
#include "Component.h"

CAnimation::CAnimation(Object* owner) : Component(owner), currentAnimation(AnimationState::None, nullptr)
{

}

void CAnimation::Awake()
{
	sprite = owner->getComponent<CSprite>();
}

void CAnimation::Update(float deltaTime)
{
	if (currentAnimation.first != AnimationState::None)
	{
		bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

		if (newFrame)
		{
			const FrameData* data = currentAnimation.second->GetCurrentFrame();

			sprite->Load(data->id); // 1   

			// We haven’t created this method yet but we’ll do that shortly.
			sprite->SetTextureRect(data->x, data->y, data->width, data->height);
		}
	}
}

void CAnimation::AddAnimation(AnimationState state, std::shared_ptr<Animation> animation)
{
	auto inserted = animations.insert(std::make_pair(state, animation));

	if (currentAnimation.first == AnimationState::None)
	{
		SetAnimationState(state);
	}
}

void CAnimation::SetAnimationState(AnimationState state)
{
	// We only set an animation of it is not already 
	// our current animation.
	if (currentAnimation.first == state)
	{
		return;
	}

	auto animation = animations.find(state);

	if (animation != animations.end())
	{
		currentAnimation.first = animation->first;
		currentAnimation.second = animation->second;

		currentAnimation.second->Reset();
	}
}

void CAnimation::SetAnimationDirection(FaceDirection direction)
{
	if (currentAnimation.first != AnimationState::None)
	{
		currentAnimation.second->SetDirection(direction);
	}
}

const AnimationState& CAnimation::GetAnimationState() const
{
	// Returns out current animation state. We can use this 
	// to compare the objects current state to a desired state.
	return currentAnimation.first;
}