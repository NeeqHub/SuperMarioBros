#include "Mario.h"

Mario::Mario(SharedContext& context) : context(context), Object(&context)
{

}

void Mario::OnCreate()
{
	auto sprite = this->addComponent<CSprite>();
	auto collider = this->addComponent<CBoxCollider>();
	auto camera = this->addComponent<CCamera>();
	auto animation = this->addComponent<CAnimation>();
	auto movement = this->addComponent<CMovementAnimation>();
	this->addComponent<CKeyboardMovement>();
	this->addComponent<OutputColliders>();
	this->addComponent<CVelocity>();
	
	int marioTextureID = context.textureAllocator->add(context.workingDir->Get() + "mario.png");

	sprite->Load(context.workingDir->Get() + "mario.png");
	sprite->SetScale(3, 3);

	this->transform->setPosition(100.0f, 500.0f);

	collider->SetTag(Tag::Player);
	
	//mario->addComponent<KillEnemy>();
	camera->SetWindow(context.window);
	sprite->Load(context.workingDir->Get() + "mario.png");
	sprite->SetScale(3, 3);

	this->transform->setPosition(100.0f, 500.0f);

	std::cout << "mario id: " << this->getComponent<C_InstanceID>()->Get() << std::endl;

	const int frameWidth = 16;
	const int frameHeight = 16;

	const int frameWidthBigMario = 16;
	const int frameHeightBigMario = 32;

	// small mario animations
	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::None);
	std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> jumpAnimation = std::make_shared<Animation>(FaceDirection::Right);

	// big mario animations
	std::shared_ptr<Animation> idleAnimationBigMario = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> walkAnimationBigMario = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> jumpAnimationBigMario = std::make_shared<Animation>(FaceDirection::Right);

	// How long we want to show each frame.
	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.06f;
	const float jumpAnimFrameSeconds = 0.02f;

	// small mario animations

	idleAnimation->AddFrame(marioTextureID, 0, 0, frameHeight, frameWidth, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 16, 0, frameHeight, frameWidth, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 32, 0, frameHeight, frameWidth, idleAnimFrameSeconds);

	walkAnimation->AddFrame(marioTextureID, 0, 16, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(marioTextureID, 16, 16, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(marioTextureID, 32, 16, frameHeight, frameWidth, walkAnimFrameSeconds);

	jumpAnimation->AddFrame(marioTextureID, 0, 32, frameHeight, frameWidth, jumpAnimFrameSeconds);
	jumpAnimation->AddFrame(marioTextureID, 16, 32, frameHeight, frameWidth, jumpAnimFrameSeconds);
	jumpAnimation->AddFrame(marioTextureID, 32, 32, frameHeight, frameWidth, jumpAnimFrameSeconds);

	deathAnimation->AddFrame(marioTextureID, 0, 48, frameHeight, frameWidth, idleAnimFrameSeconds);
	deathAnimation->AddFrame(marioTextureID, 16, 48, frameHeight, frameWidth, idleAnimFrameSeconds);
	deathAnimation->AddFrame(marioTextureID, 32, 48, frameHeight, frameWidth, idleAnimFrameSeconds);

	// big mario animations

	idleAnimationBigMario->AddFrame(marioTextureID, 0, 64, frameHeightBigMario, frameWidthBigMario, idleAnimFrameSeconds);
	idleAnimationBigMario->AddFrame(marioTextureID, 16, 64, frameHeightBigMario, frameWidthBigMario, idleAnimFrameSeconds);
	idleAnimationBigMario->AddFrame(marioTextureID, 32, 64, frameHeightBigMario, frameWidthBigMario, idleAnimFrameSeconds);

	walkAnimationBigMario->AddFrame(marioTextureID, 0, 96, frameHeightBigMario, frameWidthBigMario, walkAnimFrameSeconds);
	walkAnimationBigMario->AddFrame(marioTextureID, 16, 96, frameHeightBigMario, frameWidthBigMario, walkAnimFrameSeconds);
	walkAnimationBigMario->AddFrame(marioTextureID, 32, 96, frameHeightBigMario, frameWidthBigMario, walkAnimFrameSeconds);

	jumpAnimationBigMario->AddFrame(marioTextureID, 0, 128, frameHeightBigMario, frameWidthBigMario, jumpAnimFrameSeconds);
	jumpAnimationBigMario->AddFrame(marioTextureID, 16, 128, frameHeightBigMario, frameWidthBigMario, jumpAnimFrameSeconds);
	jumpAnimationBigMario->AddFrame(marioTextureID, 32, 128, frameHeightBigMario, frameWidthBigMario, jumpAnimFrameSeconds);

	// This adds the idle animation that we’ve just built. 
	// It will also set it as our active animation.

	animation->AddAnimation(AnimationState::Idle, idleAnimation);
	animation->AddAnimation(AnimationState::Walk, walkAnimation);
	animation->AddAnimation(AnimationState::Jump, jumpAnimation);
	animation->AddAnimation(AnimationState::Death, deathAnimation);

	animation->AddAnimation(AnimationState::IdleBig, idleAnimationBigMario);
	animation->AddAnimation(AnimationState::WalkBig, walkAnimationBigMario);
	animation->AddAnimation(AnimationState::JumpBig, jumpAnimationBigMario);

	/*******/
	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Player);

	movement->SetMarioState(this);
	//objects->add(mario);
}

void Mario::Promote()
{
	if (marioState == MarioState::SmallMario)
	{
		marioState = MarioState::BigMario;
		SetBigMarioCollider();
	}
	else if (marioState == MarioState::BigMario)
		marioState = MarioState::FireMario;

}

void Mario::Demote()
{
	if (marioState == MarioState::FireMario)
		marioState = MarioState::BigMario;
	else if (marioState == MarioState::BigMario)
		marioState = MarioState::SmallMario;
}

MarioState Mario::GetMarioState()
{
	return marioState;
}

void Mario::SetBigMarioCollider()
{
	auto collider = this->getComponent<CBoxCollider>();
	collider->SetCollidable(sf::FloatRect(0, 0, 48, 96));
}
