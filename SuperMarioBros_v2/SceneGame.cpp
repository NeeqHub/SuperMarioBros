#include "SceneGame.h"
#include "SFML/Audio.hpp"

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

void SceneGame::setSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
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
	auto camera = mario->addComponent<CCamera>();
	mario->addComponent<OutputColliders>();
	mario->addComponent<CVelocity>();
	mario->addComponent<CMovementAnimation>();
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

	std::shared_ptr<Animation> deathAnimation = std::make_shared<Animation>(FaceDirection::None);
	std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> jumpAnimation = std::make_shared<Animation>(FaceDirection::Right);

	// How long we want to show each frame.
	const float idleAnimFrameSeconds = 0.1f;
	const float walkAnimFrameSeconds = 0.06f;
	const float jumpAnimFrameSeconds = 0.02f;


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
	auto velocity = enemy1->addComponent<CVelocity>();
	auto enemyMovement = enemy1->addComponent<EnemyMovement>();
	enemy1->addComponent<EnemyAnim>();
	sprite->Load(workingDir.Get() + "mushroom.png");
	sprite->SetScale(3, 3);

	std::shared_ptr<Object> enemy2 = std::make_shared<Object>(&context);
	auto sprite2 = enemy2->addComponent<CSprite>();
	auto collider2 = enemy2->addComponent<CBoxCollider>();
	collider2->SetTag(Tag::Enemy);
	auto velocity2 = enemy2->addComponent<CVelocity>();
	auto enemyMovement2 = enemy2->addComponent<EnemyMovement>();
	enemy2->addComponent<EnemyAnim>();
	sprite2->Load(workingDir.Get() + "mushroom.png");
	sprite2->SetScale(3, 3);

	std::shared_ptr<Object> enemy4 = std::make_shared<Object>(&context);
	auto sprite4 = enemy4->addComponent<CSprite>();
	auto collider4 = enemy4->addComponent<CBoxCollider>();
	collider4->SetTag(Tag::Enemy);
	auto velocity4 = enemy4->addComponent<CVelocity>();
	auto enemyMovement4 = enemy4->addComponent<EnemyMovement>();
	enemy4->addComponent<EnemyAnim>();
	sprite4->Load(workingDir.Get() + "mushroom.png");
	sprite4->SetScale(3, 3);

	std::shared_ptr<Object> enemy3 = std::make_shared<Object>(&context);
	auto sprite3 = enemy3->addComponent<CSprite>();
	auto collider3 = enemy3->addComponent<CBoxCollider>();
	collider3->SetTag(Tag::EnemyTurtle);
	auto velocity3 = enemy3->addComponent<CVelocity>();
	auto enemyMovement3 = enemy3->addComponent<EnemyTurtleMovement>();
	enemy3->addComponent<EnemyTurtleAnim>();
	sprite3->Load(workingDir.Get() + "turtle.png");
	sprite3->SetScale(3, 3);

	std::shared_ptr<Object> enemy5 = std::make_shared<Object>(&context);
	auto sprite5 = enemy5->addComponent<CSprite>();
	auto collider5 = enemy5->addComponent<CBoxCollider>();
	collider5->SetTag(Tag::EnemyTurtle);
	auto velocity5 = enemy5->addComponent<CVelocity>();
	auto enemyMovement5 = enemy5->addComponent<EnemyTurtleMovement>();
	enemy5->addComponent<EnemyTurtleAnim>();
	sprite5->Load(workingDir.Get() + "turtle.png");
	sprite5->SetScale(3, 3);

	std::shared_ptr<Object> enemy6 = std::make_shared<Object>(&context);
	auto sprite6 = enemy6->addComponent<CSprite>();
	auto collider6 = enemy6->addComponent<CBoxCollider>();
	collider6->SetTag(Tag::Enemy);
	auto velocity6 = enemy6->addComponent<CVelocity>();
	auto enemyMovement6 = enemy6->addComponent<EnemyMovement>();
	enemy6->addComponent<EnemyAnim>();
	sprite6->Load(workingDir.Get() + "mushroom.png");
	sprite6->SetScale(3, 3);

	std::cout << "enemy1 id: " << enemy1->getComponent<C_InstanceID>()->Get() << std::endl;
	std::cout << "enemy2 id: " << enemy2->getComponent<C_InstanceID>()->Get() << std::endl;
	std::cout << "enemy3 id: " << enemy3->getComponent<C_InstanceID>()->Get() << std::endl;
	std::cout << "enemy4 id: " << enemy4->getComponent<C_InstanceID>()->Get() << std::endl;
	std::cout << "enemy5 id: " << enemy5->getComponent<C_InstanceID>()->Get() << std::endl;
	std::cout << "enemy6 id: " << enemy6->getComponent<C_InstanceID>()->Get() << std::endl;

	enemy1->transform->setPosition(31.0f * 48.0f, 500.0f);
	enemy2->transform->setPosition(41.0f * 48.0f, 500.0f);
	enemy3->transform->setPosition(95.0f * 48.0f, 500.0f);
	enemy4->transform->setPosition(120.0f * 48.0f, 500.0f);
	enemy5->transform->setPosition(51.0f * 48.0f, 500.0f);
	enemy6->transform->setPosition(108.0f * 48.0f, 500.0f);

	// Animations setups **************************************************************************************************

	auto animation = enemy1->addComponent<CAnimation>();
	auto animation2 = enemy2->addComponent<CAnimation>();
	auto animation4 = enemy4->addComponent<CAnimation>();
	auto animation3 = enemy3->addComponent<CAnimation>();
	auto animation5 = enemy5->addComponent<CAnimation>();
	auto animation6 = enemy6->addComponent<CAnimation>();

	int enemyTextureID = textureAllocator.add(workingDir.Get() + "mushroom.png");
	int enemyTurtleTextureID = textureAllocator.add(workingDir.Get() + "turtle.png");

	const int frameWidth = 16;
	const int frameHeight = 16;

	const int frameWidthTurtle = 16;
	const int frameHeightTurtle = 24;

	// Mushrooms animations

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

	animation2->AddAnimation(AnimationState::Walk, walkAnimation);
	animation2->AddAnimation(AnimationState::Death, deathAnimation);

	animation4->AddAnimation(AnimationState::Walk, walkAnimation);
	animation4->AddAnimation(AnimationState::Death, deathAnimation);

	animation6->AddAnimation(AnimationState::Walk, walkAnimation);
	animation6->AddAnimation(AnimationState::Death, deathAnimation);

	// Turtle animations

	std::shared_ptr<Animation> walkAnimationTurtle = std::make_shared<Animation>(FaceDirection::Right);
	std::shared_ptr<Animation> deathAnimationTurtle = std::make_shared<Animation>(FaceDirection::Right);

	walkAnimationTurtle->AddFrame(enemyTurtleTextureID, 0, 0, frameHeightTurtle, frameWidthTurtle, walkAnimFrameSeconds);
	walkAnimationTurtle->AddFrame(enemyTurtleTextureID, 16, 0, frameHeightTurtle, frameWidthTurtle, walkAnimFrameSeconds);
	walkAnimationTurtle->AddFrame(enemyTurtleTextureID, 0, 0, frameHeightTurtle, frameWidthTurtle, walkAnimFrameSeconds);

	deathAnimationTurtle->AddFrame(enemyTurtleTextureID, 32, 0, frameHeightTurtle, frameWidthTurtle, 0.2f);
	deathAnimationTurtle->AddFrame(enemyTurtleTextureID, 32, 0, frameHeightTurtle, frameWidthTurtle, 0.2f);
	deathAnimationTurtle->AddFrame(enemyTurtleTextureID, 32, 0, frameHeightTurtle, frameWidthTurtle, 0.2f);

	animation3->AddAnimation(AnimationState::Walk, walkAnimationTurtle);
	animation3->AddAnimation(AnimationState::Death, deathAnimationTurtle);

	animation5->AddAnimation(AnimationState::Walk, walkAnimationTurtle);
	animation5->AddAnimation(AnimationState::Death, deathAnimationTurtle);

	// Colliders setups **************************************************************************************************

	collider->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider->SetLayer(CollisionLayer::Default);

	collider2->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider2->SetLayer(CollisionLayer::Default);

	collider3->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider3->SetOffset(0.0f, 12.0f);
	collider3->SetLayer(CollisionLayer::Default);

	collider4->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider4->SetLayer(CollisionLayer::Default);

	collider5->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider5->SetOffset(0.0f, 12.0f);
	collider5->SetLayer(CollisionLayer::Default);

	collider6->SetCollidable(sf::FloatRect(0, 0, 48, 48));
	collider6->SetLayer(CollisionLayer::Default);

	// Add object **************************************************************************************************

	objects.add(enemy1);
	objects.add(enemy2);
	objects.add(enemy3);
	objects.add(enemy4);
	objects.add(enemy5);
	objects.add(enemy6);
}

