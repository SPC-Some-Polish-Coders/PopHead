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
    void load(const std::string& name);
    auto get(const std::string& name) ->ResourceType&;
    void free(const std::string& name);

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
