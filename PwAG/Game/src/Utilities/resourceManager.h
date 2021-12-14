#pragma once
#include "../Rendering system/Texture/texture.h"

//singleton
class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		return instance;
	}
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	void loadTexture(const std::string& resourceName, const std::string& textureFilePath, Texture::Type type);
	const Texture& getTexture(const std::string& resourceName) const
	{
		return textures.at(resourceName);
	}
private:
	ResourceManager() = default;
	static ResourceManager instance;

	std::unordered_map<std::string, Texture> textures;
};

