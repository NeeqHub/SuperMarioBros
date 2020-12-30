#include "Mushroom.h"

Mushroom::Mushroom(SharedContext& context) : context(context), Object(&context)
{
	mushroomTextureID = context.textureAllocator->add(context.workingDir->Get() + "mushroom.png");
}

void Mushroom::OnCreate()
{
	auto sprite = this->addComponent<CSprite>();
	auto collider = this->addComponent<CBoxCollider>();
	auto animation = this->addComponent<CAnimation>();
	this->addComponent<CVelocity>();
	this->addComponent<EnemyMovement>();
	this->addComponent<EnemyAnim>();

	sprite->Load(mushroomTextureID);
	sprite->SetScale(3, 3);
	collider->SetTag(Tag::Enemy);

	std::cout << "enemy id: " << this->getComponent<C_InstanceID>()->Get() << std::endl;

	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::Right);

	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.2f;

	walkAnimation->AddFrame(mushroomTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(mushroomTextureID, 16, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(mushroomTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);

	deathAnimation->AddFrame(mushroomTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	deathAnimation->AddFrame(mushroomTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	deathAnimation->AddFrame(mushroomTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);

	animation->AddAnimation(AnimationState::Walk, walkAnimation);
	animation->AddAnimation(AnimationState::Death, deathAnimation);

	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Default);

}

void Mushroom::SetPosition(float x, float y)
{	
	this->transform->setPosition(x, y);
}