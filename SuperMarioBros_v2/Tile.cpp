#include "Tile.h"

TileInfo::TileInfo(WorkingDirectory& workDir) : workDir(workDir)
{
}


TileInfo::~TileInfo()
{
}

void TileInfo::ReadFile(const std::string filePath)
{
	std::ifstream infile(workDir.Get() + filePath);
	Tile tile;

	while (infile >> tile.x >> tile.y >> tile.textureID)
	{
		tiles.push_back(tile);
	}
}
