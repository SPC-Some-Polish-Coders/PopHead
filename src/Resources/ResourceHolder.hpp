#ifndef POPHEAD_RESOURCE_RESOURCESHOLDER_H_
#define POPHEAD_RESOURCE_RESOURCESHOLDER_H_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Resources {

template< typename Resource >
class ResourceHolder
{

};

using SoundBufferHolder = ResourceHolder< sf::SoundBuffer >;
using TextureHolder = ResourceHolder< sf::Texture >;
using FontHolder = ResourceHolder< sf::Font >;
using ShaderHolder = ResourceHolder< sf::Shader >;

}}

#endif // !POPHEAD_RESOURCE_RESOURCESHOLDER_H_
