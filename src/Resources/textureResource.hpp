#ifndef POPHEAD_RESOURCE_TEXTURERESOURCE_H_
#define POPHEAD_RESOURCE_TEXTURERESOURCE_H_

#include <string>

#include <SFML/Graphics.hpp>

#include "resource.hpp"

namespace PopHead {
namespace Resources {

class TextureResource : public Resource< TextureResource >
{
  private:
    virtual bool load( std::string path ) override;

    sf::Texture mTexture;
};

}}

#endif // !POPHEAD_RESOURCE_TEXTURERESOURCE_H_
