#include "SceneGame.h"


SceneGame::SceneGame(WorkingDirectory& workingDir,
	ResourceManager<sf::Texture>& textureAllocator,
	Window& window) :
	workingDir(workingDir),
	textureAllocator(textureAllocator),
	tileSystem(textureAllocator, context),
	window(window) { }

void SceneGame::onCreate()
{
	context.input = &input;
	context.objects = &objects;
	context.workingDir = &workingDir;
	context.textureAllocator = &textureAllocator;
	context.window = &window;

	std::vector<std::shared_ptr<Object>> levelTiles = tileSystem.LoadMapTiles("cord.txt");
	std::vector<std::shared_ptr<Object>> levelBackgroundTiles = tileSystem.LoadBackgroundTiles("cordBackground.txt");

	int blocksTextureID = textureAllocator.add(workingDir.Get() + "map_tiles.png");
	int backgroundTextureID = textureAllocator.add(workingDir.Get() + "background_map_tiles.png");

	objects.add(levelBackgroundTiles);
	objects.add(levelTiles);
	//CreateEnemy();
	CreatePlayer();
	CreateEnemy();
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
	Debug::Draw(window);
}

void SceneGame::CreatePlayer()
{
	std::shared_ptr<Object> mario = std::make_shared<Object>(&context);
	auto sprite = mario->addComponent<CSprite>();
	auto collider = mario->addComponent<CBoxCollider>();
	collider->SetTag(Tag::Player);
	auto camera = mario->addComponent<C_Camera>();
	mario->addComponent<OutputColliders>();
	mario->addComponent<C_Velocity>();
	mario->addComponent<C_MovementAnimation>();
	//mario->addComponent<KillEnemy>();
	camera->SetWindow(&window);
	sprite->Load(workingDir.Get() + "mario.png");
	sprite->SetScale(3, 3);

	mario->transform->setPosition(100.0f, 500.0f);

	std::cout << "mario id: " << mario->getComponent<C_InstanceID>()->Get() << std::endl;

	mario->addComponent<CKeyboardMovement>();

	auto animation = mario->addComponent<CAnimation>();

	int marioTextureID = textureAllocator.add(workingDir.Get() + "mario.png");

	const int frameWidth = 16;
	const int frameHeight = 16;

	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> jumpAnimation = std::make_shared<Animation>(FaceDirection::Right);

	// How long we want to show each frame.
	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.05f;
	const float jumpAnimFrameSeconds = 0.1f;


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

	// This adds the idle animation that we’ve just built. 
	// It will also set it as our active animation.
	
	animation->AddAnimation(AnimationState::Idle, idleAnimation);
	animation->AddAnimation(AnimationState::Walk, walkAnimation);
	animation->AddAnimation(AnimationState::Jump, jumpAnimation);
	animation->AddAnimation(AnimationState::Death, deathAnimation);

	/*******/
	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Player);

	//objects.add(levelTiles);
	objects.add(mario);
}

void SceneGame::CreateEnemy()
{
	std::shared_ptr<Object> enemy1 = std::make_shared<Object>(&context);
	auto sprite = enemy1->addComponent<CSprite>();
	auto collider = enemy1->addComponent<CBoxCollider>();
	collider->SetTag(Tag::Enemy);
	auto velocity = enemy1->addComponent<C_Velocity>();
	auto enemyMovement = enemy1->addComponent<EnemyMovement>();
	enemy1->addComponent<EnemyAnim>();
	sprite->Load(workingDir.Get() + "grzybki2.png");
	sprite->SetScale(3, 3);

	std::cout << "enemy id: " << enemy1->getComponent<C_InstanceID>()->Get() << std::endl;

	enemy1->transform->setPosition(31.0f * 48.0f, 500.0f);

	auto animation = enemy1->addComponent<CAnimation>();

	int enemyTextureID = textureAllocator.add(workingDir.Get() + "grzybki2.png");

	const int frameWidth = 16;
	const int frameHeight = 16;

	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::Right);

	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.2f;

	walkAnimation->AddFrame(enemyTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(enemyTextureID, 16, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	walkAnimation->AddFrame(enemyTextureID, 0, 0, frameHeight, frameWidth, walkAnimFrameSeconds);

	

	deathAnimation->AddFrame(enemyTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	deathAnimation->AddFrame(enemyTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);
	deathAnimation->AddFrame(enemyTextureID, 32, 0, frameHeight, frameWidth, walkAnimFrameSeconds);

	animation->AddAnimation(AnimationState::Walk, walkAnimation);
	animation->AddAnimation(AnimationState::Death, deathAnimation);
	

	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Default);

	//objects.add(levelTiles);
	objects.add(enemy1);
}
