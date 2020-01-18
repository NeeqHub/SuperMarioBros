#pragma once
#include "ResourceManager.h"
#include "WorkingDirectory.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Object.h"
#include "Component.h"

struct TileInfo
{
	TileInfo() : tileID(-1)
	{
	}

	TileInfo(int textureID, unsigned int tileID, sf::IntRect textureRect)
		: textureID(textureID), tileID(tileID), textureRect(textureRect) { }

	int tileID;
	int textureID;
	sf::IntRect textureRect;
};

struct Tile
{
	int tileID;
	int x;
	int y;
};

struct Layer
{
	std::vector<std::shared_ptr<Tile>> tiles;
	bool isVisible;
};

class STile
{
public:
	STile(ResourceManager<sf::Texture>& textureAllocator,
		SharedContext& context);
	~STile();
	std::vector<std::shared_ptr<Object>> LoadMapTiles(const std::string& filePath);
	std::vector<std::shared_ptr<Object>> LoadBackgroundTiles(const std::string& filePath);
	
private:
	//WorkingDirectory& workDir;
	ResourceManager<sf::Texture>& textureAllocator;
	SharedContext& context;
};







