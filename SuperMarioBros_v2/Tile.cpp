#include "Tile.h"

STile::STile(ResourceManager<sf::Texture>& textureAllocator,
	SharedContext& context) : textureAllocator(textureAllocator), context(context)
{
}


STile::~STile()
{
}

std::vector<std::shared_ptr<Object>> STile::LoadMapTiles(const std::string& filePath)
{
	std::ifstream infile(context.workingDir->Get() + filePath);
	Tile tile;
	unsigned int tileScale = 3;

	std::vector<std::shared_ptr<Object>> tileObjects;

	while (infile >> tile.x >> tile.y >> tile.tileID)
	{
		std::shared_ptr<Object> tileObject = std::make_shared<Object>(&context);
		auto sprite = tileObject->addComponent<CSprite>();
		auto collider = tileObject->addComponent<CBoxCollider>();
		sprite->Load(context.workingDir->Get() + "map_tiles.png");

		// 0 ground
		// 1 brick
		// 2 surprise
		// 3 dsurprise
		// 4 block
		// 5 coin
		// 6 top-left pipe
		// 7 top-right pipe
		// 8 bottom-left pipe
		// 9 bottom-right pipe
		
		switch (tile.tileID)
		{
		case 0: sprite->SetTextureRect(0, 0, 16, 16); break;
		case 1: sprite->SetTextureRect(16, 0, 16, 16); break;
		case 2: sprite->SetTextureRect(32, 0, 16, 16); break;
		case 4: sprite->SetTextureRect(0, 16, 16, 16); break;
		case 6: sprite->SetTextureRect(0, 32, 16, 16);  break;
		case 7: sprite->SetTextureRect(16, 32, 16, 16);  break;
		case 8: sprite->SetTextureRect(0, 48, 16, 16);  break;
		case 9: sprite->SetTextureRect(16, 48, 16, 16);  break;

		default:
			break;
		}
		
		sprite->SetScale(tileScale, tileScale);
		tileObject->transform->setPosition(tile.x * 48, tile.y * 48);

		float left = tile.x * 48.0f - (16.0f * tileScale) * 0.5f;
		float top = tile.y * 48.0f - (16.0f * tileScale) * 0.5f;
		float width = 16.0f * tileScale;
		float height = 16.0f * tileScale;

		collider->SetCollidable(sf::FloatRect(left, top, width, height));
		collider->SetTag(Tag::Defult);
		collider->SetLayer(CollisionLayer::Tile);

		tileObjects.emplace_back(tileObject);
	}

	return tileObjects;
}

std::vector<std::shared_ptr<Object>> STile::LoadBackgroundTiles(const std::string& filePath)
{
	std::ifstream infile(context.workingDir->Get() + filePath);
	Tile tile;
	unsigned int tileScale = 3;

	std::vector<std::shared_ptr<Object>> tileObjects;

	while (infile >> tile.x >> tile.y >> tile.tileID)
	{
		std::shared_ptr<Object> tileObject = std::make_shared<Object>(&context);
		auto sprite = tileObject->addComponent<CSprite>();
		sprite->Load(context.workingDir->Get() + "background_tiles.png");
		
		float x = 0;

		// 17 cloud
		
		switch (tile.tileID)
		{
		x = 0;

		case 0: sprite->SetTextureRect(0, 0, 16, 16);  break;
		case 1: sprite->SetTextureRect(16, 0, 16, 16);  break;
		case 2: sprite->SetTextureRect(32, 0, 16, 16);  break;
		case 3: sprite->SetTextureRect(48, 0, 16, 16);  break;
		case 4: sprite->SetTextureRect(0, 16, 16, 16);  break;
		case 5: sprite->SetTextureRect(16, 16, 16, 16);  break;
		case 6: sprite->SetTextureRect(32, 16, 16, 16);  break;
		case 7: sprite->SetTextureRect(48, 16, 16, 16);  break;
			
		case 8: sprite->SetTextureRect(0, 32, 16, 16);  break;
		case 9: sprite->SetTextureRect(16, 32, 16, 16);  break;
		case 10: sprite->SetTextureRect(32, 32, 16, 16);  break;
		case 12: sprite->SetTextureRect(0, 48, 16, 16);  break;
		case 13: sprite->SetTextureRect(16, 48, 16, 16);  break;

		case 15: sprite->SetTextureRect(48, 48, 16, 16);  break;
		case 11: sprite->SetTextureRect(48, 32, 16, 16);  break;
		case 17: sprite->SetTextureRect(0, 64, 32, 24);  break;

		case 18: sprite->SetTextureRect(32, 64, 32, 24);  break;

		default:
			break;
		}

		sprite->SetScale(tileScale, tileScale);
		tileObject->transform->setPosition(tile.x * 48, tile.y * 48);

		tileObjects.emplace_back(tileObject);
	}

	return tileObjects;
}
