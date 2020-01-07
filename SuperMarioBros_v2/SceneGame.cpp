#include "SceneGame.h"


SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceManager<sf::Texture>& textureAllocator) : workingDir(workingDir), textureAllocator(textureAllocator), tileSystem(workingDir, textureAllocator)
{

}

void SceneGame::onCreate()
{
	//tileinfo->ReadFile("cord.txt");
	std::vector<std::shared_ptr<Object>> levelTiles = tileSystem.ReadFile("cord.txt");

	std::shared_ptr<Object> mario = std::make_shared<Object>();
	auto sprite = mario->addComponent<CSprite>();
	auto collider = mario->addComponent<CBoxCollider>();
	sprite->SetTextureAllocator(&textureAllocator);
	sprite->Load(workingDir.Get() + "mario.png");
	sprite->SetScale(3, 3);

	auto movement = mario->addComponent<CKeyboardMovement>();
	movement->setInput(&input);
	movement->setMovementSpeed(400.0f);

	/*******/

	auto animation = mario->addComponent<CAnimation>();

	int marioTextureID = textureAllocator.add(workingDir.Get() + "mario.png");
	int blocksTextureID = textureAllocator.add(workingDir.Get() + "blocks.png");

	const int frameWidth = 16; 
	const int frameHeight = 16;

	std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);

	// How long we want to show each frame.
	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.05f;

	idleAnimation->AddFrame(marioTextureID, 0, 0, frameHeight, frameWidth, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 16, 0, frameHeight, frameWidth, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 32, 0, frameHeight, frameWidth, idleAnimFrameSeconds);

	walkAnimation->AddFrame(marioTextureID, 0, 16, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(marioTextureID, 16, 16, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(marioTextureID, 32, 16, frameHeight, frameWidth, walkAnimFrameSeconds);

	// This adds the idle animation that we’ve just built. 
	// It will also set it as our active animation.
	animation->AddAnimation(AnimationState::Idle, idleAnimation);
	animation->AddAnimation(AnimationState::Walk, walkAnimation);

	/*******/
	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Player);

	objects.add(levelTiles);
	objects.add(mario);

}

void SceneGame::onDestroy()
{

}

void SceneGame::processInput()
{
	input.update();
}

void SceneGame::Update(float deltaTime)
{
	objects.processRemovals();
	objects.processNewObjects();
	objects.Update(deltaTime);
}

void SceneGame::LateUpdate(float deltaTime)
{
	objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
	objects.Draw(window);
}
