#ifndef POPHEAD_RESOURCE_RESOURCESHOLDER_H_
#define POPHEAD_RESOURCE_RESOURCESHOLDER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "Resources/resource.hpp"
#include "Resources/textureResource.hpp"
#include "Resources/fontResource.hpp"
#include "Resources/shaderResource.hpp"
#include "Resources/soundBufferResource.hpp"

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

  private:
    ResourceMap mResources;
};

using SoundBufferHolder = ResourceHolder< SoundBufferResource >;
using TextureHolder = ResourceHolder< TextureResource >;
using FontHolder = ResourceHolder< FontResource >;
using ShaderHolder = ResourceHolder< ShaderResource >;

}}

#endif // !POPHEAD_RESOURCE_RESOURCESHOLDER_H_
