#ifndef POPHEAD_RESOURCES_RESOURCESHOLDER_H_
#define POPHEAD_RESOURCES_RESOURCESHOLDER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace PopHead {
namespace Resources {


template< typename ResourceType >
class ResourceHolder
{
public:
    void load(const std::string& filePath);
    auto get(const std::string& filePath) ->ResourceType&;
    void free(const std::string& filePath);
//private:
//	auto getFullFilePath(const std::string& filePath)->std::string{ return "resources/" + filePath; }

private:
	std::unordered_map< std::string, std::unique_ptr<ResourceType> > mResources;
};

using SoundBufferHolder = ResourceHolder< sf::SoundBuffer >;
using TextureHolder = ResourceHolder< sf::Texture >;
using FontHolder = ResourceHolder< sf::Font >;
using ShaderHolder = ResourceHolder< sf::Shader>;


}}

#include "Resources/resourceHolder.inl"

#endif // !POPHEAD_RESOURCES_RESOURCESHOLDER_H_
