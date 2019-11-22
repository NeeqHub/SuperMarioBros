#include "ResourceHolder.h"

template<class Resource, class Identifier>
ResourceHolder<Resource, Identifier>::ResourceHolder()
{
}

template<class Resource, class Identifier>
ResourceHolder<Resource, Identifier>::~ResourceHolder()
{
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filePath)
{
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filePath))
	throw std::runtime_error("ResourceHolder::load failed - filepath: " + filePath);

	auto inserted = GameTextures.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = GameTextures.find(id);
	return *found->second;
}