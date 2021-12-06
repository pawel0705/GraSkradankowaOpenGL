#include "pch.h"
#include "resourceManager.h"

ResourceManager ResourceManager::instance;

void ResourceManager::loadTexture(const std::string& resourceName, const std::string& textureFilePath, TextureType type)
{
	textures.insert(std::make_pair(resourceName, Texture(textureFilePath, type)));
}
