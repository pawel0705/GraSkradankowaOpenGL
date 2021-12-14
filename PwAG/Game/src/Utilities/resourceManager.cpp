#include "pch.h"
#include "resourceManager.h"

ResourceManager ResourceManager::instance;

void ResourceManager::loadTexture(const std::string& resourceName, const std::string& textureFilePath, Texture::Type type)
{
	textures.insert(std::make_pair(resourceName, Texture::createTextureFromFile(textureFilePath, type)));
}
