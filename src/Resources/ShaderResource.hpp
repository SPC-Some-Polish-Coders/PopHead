#ifndef POPHEAD_RESOURCE_SHADERRESOURCE_H_
#define POPHEAD_RESOURCE_SHADERRESOURCE_H_

#include <SFML/Graphics.hpp>

#include "Resources/Resource.hpp"

namespace PopHead {
namespace Resources {

class ShaderResource : public Resource< ShaderResource >
{
  private:
    virtual bool load() override;

    sf::Shader mShader;
};

}}

#endif // !POPHEAD_RESOURCE_SHADERRESOURCE_H_
