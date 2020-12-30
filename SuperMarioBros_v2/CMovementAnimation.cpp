#include "CMovementAnimation.h"
#include "Mario.h"

CMovementAnimation::CMovementAnimation(Object* owner) : Component(owner) { }

void CMovementAnimation::Awake()
{
	velocity = owner->getComponent<CVelocity>();
	animation = owner->getComponent<CAnimation>();
}

void CMovementAnimation::Update(float deltaTime)
{
	if (animation->GetAnimationState() != AnimationState::Projectile)
	{
		if (owner->transform->getPosition().y >= 550.0f)
		{
			owner->hitted = true;
			owner->disableInput = true;
			return;
		}

		const sf::Vector2f& currentVel = velocity->Get();

		if (owner->hitted == true)
		{	
			animation->SetAnimationState(AnimationState::Death);
			owner->getComponent<CBoxCollider>()->SetSize(0.0f, 0.0f);
			owner->disableInput = true;
			return;
		}

		if (owner->transform->canJump == true && currentVel.x != 0.0f)
		{
			// 30.12.2021 update DL

			//std::cout << static_cast<int>(myMario->GetMarioState()) << std::endl;

			if(myMario->GetMarioState() == MarioState::SmallMario)
			animation->SetAnimationState(AnimationState::Walk);
			else if (myMario->GetMarioState() == MarioState::BigMario)
				animation->SetAnimationState(AnimationState::WalkBig);

			// -----

			if (currentVel.x < 0)
			{
				animation->SetAnimationDirection(FaceDirection::Left);
			}
			else
			{
				animation->SetAnimationDirection(FaceDirection::Right);
			}

		}
		else if (owner->transform->canJump == false)
		{
			if (myMario->GetMarioState() == MarioState::SmallMario)
				animation->SetAnimationState(AnimationState::Jump);
			else if (myMario->GetMarioState() == MarioState::BigMario)
				animation->SetAnimationState(AnimationState::JumpBig);

			if (currentVel.x < 0)
				animation->SetAnimationDirection(FaceDirection::Left);
			else
				animation->SetAnimationDirection(FaceDirection::Right);
		}
		else
		{
			if (myMario->GetMarioState() == MarioState::SmallMario)
				animation->SetAnimationState(AnimationState::Idle);
			else if (myMario->GetMarioState() == MarioState::BigMario)
				animation->SetAnimationState(AnimationState::IdleBig);
		}
	}
}

void CMovementAnimation::SetMarioState(Mario* mario)
{
	myMario = mario;
}