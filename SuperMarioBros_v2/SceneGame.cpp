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

	// Create mario 
	std::shared_ptr<Mario> mario = std::make_shared<Mario>(context);
	mario->OnCreate();
	//mario->Promote();

	// Create mushrooms
	std::shared_ptr<Mushroom> mushroom1 = std::make_shared<Mushroom>(context);
	mushroom1->OnCreate();
	mushroom1->SetPosition(31.0f * 48.0f, 500.0f);

	std::shared_ptr<Mushroom> mushroom2 = std::make_shared<Mushroom>(context);
	mushroom2->OnCreate();
	mushroom2->SetPosition(41.0f * 48.0f, 500.0f);

	// Create turtles
	std::shared_ptr<Turtle> turtle1 = std::make_shared<Turtle>(context);
	turtle1->OnCreate();
	turtle1->SetPosition(51.0f * 48.0f, 500.0f);

	std::shared_ptr<Turtle> turtle2 = std::make_shared<Turtle>(context);
	turtle2->OnCreate();
	turtle2->SetPosition(90.0f * 48.0f, 500.0f);

	// Add objects to ObjectCollection
	objects.add(levelBackgroundTiles);
	objects.add(levelTiles);
	objects.add(mario);
	objects.add(mushroom1);
	objects.add(mushroom2);
	objects.add(turtle1);
	objects.add(turtle2);
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
	//Debug::Draw(window);
}