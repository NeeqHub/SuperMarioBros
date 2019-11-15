#pragma once
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <memory>

namespace Textures
{
	enum ID { Mario, Blocks, Enemy, Powerups };
}

template<class Resource, class Identifier>
class ResourceHolder
{
public:

	ResourceHolder();
	~ResourceHolder();

	void load(Identifier id, const std::string& filePath);
	Resource& get(Identifier id);

private:
	std::map<Identifier, std::unique_ptr<Resource>> GameTextures;
	
};

