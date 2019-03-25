#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include "World/Entity/entity.hpp"
#include "Renderer/layerID.hpp"

namespace PopHead {
namespace World {
namespace Entity {

class Object : public Entity
{
  public:
    Object( Renderer::LayerID layerID );
    virtual void draw() =0;
    // virtual void draw( sf::RenderTarget&, sf::RenderStates ) =0;

    inline auto getLayerID() const -> Renderer::LayerID;

  private:
    const Renderer::LayerID mLayerID;
};

auto Object::getLayerID() const -> Renderer::LayerID
{ return mLayerID; }

}}}

#endif // !POPHEAD_WORLD_OBJECT_H_
