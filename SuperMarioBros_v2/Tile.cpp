#include "Tile.h"

STile::STile(WorkingDirectory& workDir, ResourceManager<sf::Texture>& textureAllocator) : workDir(workDir), textureAllocator(textureAllocator)
{
}


STile::~STile()
{
}

std::vector<std::shared_ptr<Object>> STile::ReadFile(const std::string& filePath)
{
	std::ifstream infile(workDir.Get() + filePath);
	Tile tile;
	unsigned int tileScale = 3;

	std::vector<std::shared_ptr<Object>> tileObjects;

	while (infile >> tile.x >> tile.y >> tile.tileID)
	{
		std::shared_ptr<Object> tileObject = std::make_shared<Object>();
		auto sprite = tileObject->addComponent<CSprite>();
		auto collider = tileObject->addComponent<CBoxCollider>();
		sprite->SetTextureAllocator(&textureAllocator);
		sprite->Load(workDir.Get() + "blocks.png");

		// 1 ground
		// 2 brick
		// 3 surprise
		// 4 dsurprise
		
		switch (tile.tileID)
		{
		case 0: sprite->SetTextureRect(0, 0, 16, 16); break;
		case 1: sprite->SetTextureRect(16, 0, 16, 16); break;
		case 2: sprite->SetTextureRect(32, 0, 16, 16); break;

		default:
			break;
		}
		
		sprite->SetScale(tileScale, tileScale);
		tileObject->transform->setPosition(tile.x * 48, tile.y * 48);

		float left = tile.x - (16.0f * tileScale) * 0.5f;
		float top = tile.y - (16.0f * tileScale) * 0.5f;
		float width = 16.0f * tileScale;
		float height = 16.0f * tileScale;

		collider->SetCollidable(sf::FloatRect(left, top, width, height));
		collider->SetLayer(CollisionLayer::Tile);

		tileObjects.emplace_back(tileObject);
	}

	return tileObjects;
}
