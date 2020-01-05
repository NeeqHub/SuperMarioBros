#pragma once
#include "ResourceManager.h"
#include "WorkingDirectory.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Object.h"

// Stores common tile data.
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
	//std::shared_ptr<TileInfo> properties;
	int tileID;
	int x;
	int y;
};

//using TileSet = std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>;

/*struct TileSheetData
{
	// The texture id will be retrieved by using our texture allocator.
	int textureId; // The id of the tile sets texture. 
	sf::Vector2u imageSize; // The size of the texture.
	int columns; // How many columns in the tile sheet.
	int rows; // How many rows in the tile sheet.
	sf::Vector2u tileSize; // The size of an individual tile.
};*/

struct Layer
{
	std::vector<std::shared_ptr<Tile>> tiles;
	bool isVisible;
};

class STile
{
public:
	STile(WorkingDirectory& workDir, ResourceManager<sf::Texture>& textureAllocator);
	~STile();
	std::vector<std::shared_ptr<Object>> ReadFile(const std::string& filePath);
	
private:
	WorkingDirectory& workDir;
	ResourceManager<sf::Texture>& textureAllocator;
};







