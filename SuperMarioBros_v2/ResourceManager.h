#pragma once
#include <map>
#include <memory>
#include <string>
#include <iostream>

template <class T>
class ResourceManager
{
public:
	ResourceManager();
	
	int add(const std::string& filePath)
	{
		auto it = resources.find(filePath);

		if (it != resources.end())
		{
			return it->second.first;
		}

		std::shared_ptr<T> resource = std::make_shared<T>();
		if (!resource->loadFromFile(filePath))
			return -1;

		resources.insert(make_pair(filePath,std::make_pair(currentID, resource)));
		
		std::cout << currentID + 1 << std::endl;
		return currentID++;
	}

	void remove(int id)
	{
		for (auto it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second.first == id)
				resources.erase(it->first);
		}
	}

	std::shared_ptr<T> get(int id)
	{
		for (auto it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second.first == id)
				return it->second.second;
		}

		return nullptr;
	}

	bool has(int id)
	{
		return (get(id) != nullptr);
	}

private:
	int currentID;
	std::map<std::string, std::pair<int, std::shared_ptr<T>>> resources;
};

template<class T>
inline ResourceManager<T>::ResourceManager()
{
}
