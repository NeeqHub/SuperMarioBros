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

	std::vector<std::shared_ptr<Object>> tileObjects;

	while (infile >> tile.x >> tile.y >> tile.tileID)
	{
		std::shared_ptr<Object> tileObject = std::make_shared<Object>();
		auto sprite = tileObject->addComponent<CSprite>();
		sprite->SetTextureAllocator(&textureAllocator);
		sprite->Load(workDir.Get() + "blocks.png");
		sprite->SetTextureRect(0, 0, 16, 16);
		tileObject->transform->setPosition(tile.x * 48, tile.y * 48);

		tileObjects.emplace_back(tileObject);
	}

	return tileObjects;
}
