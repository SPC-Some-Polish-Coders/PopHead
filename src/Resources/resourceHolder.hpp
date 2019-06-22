#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ph {

template< typename ResourceType >
class ResourceHolder
{
public:
    void load(const std::string& filePath);
    auto get(const std::string& filePath) ->ResourceType&;
    void free(const std::string& filePath);

private:
	std::unordered_map< std::string, std::unique_ptr<ResourceType> > mResources;
};

using SoundBufferHolder = ResourceHolder< sf::SoundBuffer >;
using TextureHolder = ResourceHolder< sf::Texture >;
using FontHolder = ResourceHolder< sf::Font >;
using ShaderHolder = ResourceHolder< sf::Shader>;

}

#include "Resources/resourceHolder.inl"
