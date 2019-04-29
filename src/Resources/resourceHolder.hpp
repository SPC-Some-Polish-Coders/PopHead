#ifndef POPHEAD_RESOURCES_RESOURCESHOLDER_H_
#define POPHEAD_RESOURCES_RESOURCESHOLDER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace PopHead {
namespace Resources {

template< typename Resource >
class ResourceHolder
{
public:
    using ResourceMap =
            std::unordered_map< std::string, std::unique_ptr< Resource > >;

    inline auto get( const std::string& name ) -> Resource&;
    inline void free( const std::string& name );
    inline bool has( const std::string& name );
    inline bool load( const std::string& name );

private:
    ResourceMap mResources;
};

using SoundBufferHolder = ResourceHolder< sf::SoundBuffer >;
using TextureHolder = ResourceHolder< sf::Texture >;
using FontHolder = ResourceHolder< sf::Font >;
using ShaderHolder = ResourceHolder< sf::Shader>;

}}

#endif // !POPHEAD_RESOURCES_RESOURCESHOLDER_H_

#include "Resources/resourceHolder.inl"
