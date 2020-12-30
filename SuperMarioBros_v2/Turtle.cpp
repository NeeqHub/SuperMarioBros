#include "Turtle.h"

Turtle::Turtle(SharedContext& context) : context(context), Object(&context)
{
	turtleTextureID = context.textureAllocator->add(context.workingDir->Get() + "turtle.png");
}

void Turtle::OnCreate()
{
	auto sprite = this->addComponent<CSprite>();
	auto collider = this->addComponent<CBoxCollider>();
	auto animation = this->addComponent<CAnimation>();
	this->addComponent<CVelocity>();
	this->addComponent<EnemyTurtleMovement>();
	this->addComponent<EnemyTurtleAnim>();

	sprite->Load(turtleTextureID);
	sprite->SetScale(3, 3);
	collider->SetTag(Tag::EnemyTurtle);

	std::cout << "enemy id: " << this->getComponent<C_InstanceID>()->Get() << std::endl;

	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::Right);

	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.2f;

	walkAnimation->AddFrame(turtleTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(turtleTextureID, 16, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(turtleTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);

	deathAnimation->AddFrame(turtleTextureID, 32, 0, frameHeight, frameWidth, 0.2f);
	deathAnimation->AddFrame(turtleTextureID, 32, 0, frameHeight, frameWidth, 0.2f);
	deathAnimation->AddFrame(turtleTextureID, 32, 0, frameHeight, frameWidth, 0.2f);

	animation->AddAnimation(AnimationState::Walk, walkAnimation);
	animation->AddAnimation(AnimationState::Death, deathAnimation);

	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Default);
	collider->SetOffset(0.0f, 12.0f);

}

void Turtle::SetPosition(float x, float y)
{
	this->transform->setPosition(x, y);
}