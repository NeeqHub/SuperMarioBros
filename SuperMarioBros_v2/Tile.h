#pragma once
#include "ResourceManager.h"
#include "WorkingDirectory.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>

struct Tile
{
	int textureID;
	int x;
	int y;
};

class TileInfo
{
public:
	TileInfo(WorkingDirectory& workDir);
	~TileInfo();
	void ReadFile(const std::string filePath);

	
private:
	WorkingDirectory& workDir;
	std::vector<Tile> tiles;
};





