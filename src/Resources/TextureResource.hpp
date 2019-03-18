#ifndef POPHEAD_RESOURCE_TEXTURERESOURCE_H_
#define POPHEAD_RESOURCE_TEXTURERESOURCE_H_

#include <SFML/Graphics.hpp>

#include "Resources/Resource.hpp"

namespace PopHead {
namespace Resources {

class TextureResource : public Resource< TextureResource >
{
  private:
    virtual bool load() override;

    sf::Texture mTexture;
};

}}

#endif // !POPHEAD_RESOURCE_TEXTURERESOURCE_H_
