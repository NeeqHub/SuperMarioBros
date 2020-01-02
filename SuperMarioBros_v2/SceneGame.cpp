#include "SceneGame.h"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceManager<sf::Texture>& textureAllocator) : workingDir(workingDir), textureAllocator(textureAllocator)
{

}

void SceneGame::onCreate()
{
	std::shared_ptr<Object> mario = std::make_shared<Object>();
	auto sprite = mario->addComponent<CSprite>();
	sprite->SetTextureAllocator(&textureAllocator);
	sprite->Load(workingDir.Get() + "mario.png");

	auto movement = mario->addComponent<CKeyboardMovement>();
	movement->setInput(&input);

	/*******/

	auto animation = mario->addComponent<CAnimation>();

	int marioTextureID = textureAllocator.add(workingDir.Get() + "Viking.png");

	const int frameWidth = 165; //1
	const int frameHeight = 145;

	std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>();//2

	// How long we want to show each frame.
	const float idleAnimFrameSeconds = 0.2f;

	idleAnimation->AddFrame(marioTextureID, 600, 0, frameWidth, frameHeight, idleAnimFrameSeconds);//3
	idleAnimation->AddFrame(marioTextureID, 800, 0, frameWidth, frameHeight, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 0, 145, frameWidth, frameHeight, idleAnimFrameSeconds);
	idleAnimation->AddFrame(marioTextureID, 200, 145, frameWidth, frameHeight, idleAnimFrameSeconds);

	// This adds the idle animation that we’ve just built. 
	// It will also set it as our active animation.
	animation->AddAnimation(AnimationState::Idle, idleAnimation);

	/*******/

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
	objects.processNewObjects();
	objects.Update(deltaTime);
	//mario->update(deltaTime);
	/*const sf::Vector2f& spritePos = sprite.getPosition();
	const int moveSpeed = 100;

	int xMove = 0;
	int yMove = 0;

	if (input.isKeyPressed(Input::Key::Left))
	{
		xMove = -moveSpeed;
	}
	else if (input.isKeyPressed(Input::Key::Right))
	{
		xMove = moveSpeed;
	}

	if (input.isKeyPressed(Input::Key::Up))
	{
		yMove = -moveSpeed;
	}
	else if (input.isKeyPressed(Input::Key::Down))
	{
		yMove = moveSpeed;
	}

	float xFrameMove = xMove * deltaTime;
	float yFrameMove = yMove * deltaTime;

	sprite.setPosition(spritePos.x + xFrameMove, spritePos.y + yFrameMove);*/
}

void SceneGame::LateUpdate(float deltaTime)
{
	objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
	objects.Draw(window);
}
