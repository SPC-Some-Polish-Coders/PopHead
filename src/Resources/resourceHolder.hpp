#pragma once

#include "Renderer/API/texture.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace ph {

template< typename ResourceType >
class ResourceHolder
{
public:
    bool load(const std::string& filePath);
    auto get(const std::string& filePath) -> ResourceType&;
    bool free(const std::string& filePath);

private:
	std::unordered_map< std::string, std::unique_ptr<ResourceType> > mResources;
};

using SoundBufferHolder = ResourceHolder<sf::SoundBuffer>;
using TextureHolder = ResourceHolder<ph::Texture>;
using OldFontHolder = ResourceHolder<sf::Font>;

}

#include "Resources/resourceHolder.inl"
