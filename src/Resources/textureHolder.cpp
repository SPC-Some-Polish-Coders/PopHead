#include "pch.hpp"
#include "textureHolder.hpp"
#include "Renderer/API/texture.hpp"

namespace ph {

static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

bool loadTexture(const std::string& filePath, bool srgb)
{
	std::string fullFilePath = "resources/" + filePath;
	if(textures.find(fullFilePath) != textures.end())
		return true;
	auto texture = std::make_unique<Texture>();
	if(texture->loadFromFile(fullFilePath, srgb))
	{
		textures.insert(std::make_pair(fullFilePath, std::move(texture)));
		return true;
	}
	else
	{
		PH_LOG_ERROR("unable to load texture \"" + fullFilePath + "\"");
		return false;
	}
}

Texture& getTexture(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	auto found = textures.find(fullFilePath);
	PH_ASSERT_CRITICAL(found != textures.end(), "You try to get a texture that wasn't loaded: " + fullFilePath);
	return *found->second;
}

}

