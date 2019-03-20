#ifndef POPHEAD_RENDERER_RENDERER_H_
#define POPHEAD_RENDERER_RENDERER_H_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "Renderer/Layer.hpp"
#include "Renderer/LayerID.hpp"

namespace PopHead {
namespace Renderer {

/// Renderer class.
class Renderer
{
  public:
    Renderer();
    Renderer( Renderer& ) = delete;
    Renderer& operator=( Renderer& ) = delete;

    void draw() const;

    void addObject( World::Entity::Object* const objectToAdd );
    void addObject( World::Entity::Object* const objectToAdd,
                    LayerID layerID );

    void removeObject( const World::Entity::Object* const object );
    void removeObject( std::string name, LayerID layerID );

    inline auto getWindow() const -> const sf::Window&;
    inline auto getLayer( LayerID ) -> Layer&;
    inline auto getLayer( LayerID ) const -> const Layer&;

  private:
    sf::View mCamera;
    // std::vector< sf::Rect< float > > mViewports;
    mutable sf::RenderWindow mWindow;
    mutable std::map< LayerID, Layer > mLayers;
};

auto Renderer::getWindow() const -> const sf::Window&
{ return mWindow; }

auto Renderer::getLayer( LayerID id ) -> Layer&
{ return mLayers[id]; }

auto Renderer::getLayer( LayerID id ) const -> const Layer&
{ return mLayers[id]; }

}}

#endif // !POPHEAD_RENDERER_RENDERER_H_
