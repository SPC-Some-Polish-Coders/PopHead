#ifndef POPHEAD_RENDERER_RENDERER_H_
#define POPHEAD_RENDERER_RENDERER_H_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "Renderer/layer.hpp"
#include "Renderer/layerID.hpp"

/** @defgroup Renderer Renderer
 *  Renderer module manages window management and drawing.
 *  
 *  Object must be add to [Renderer](@ref PopHead::Renderer::Renderer), before anything is drawn. It is archived by
 *  Renderer::addObject methods.
 *  
 *  Objects are stored in Layers, not directly in Renderer.
 *  
 *  Renderer consists of few Layers. Each layer is drawn separately in
 *  pre-defined order. */

namespace PopHead {

/// @ingroup Renderer
namespace Renderer {

/// @ingroup Renderer
class Renderer
{
  public:
    Renderer();

    /// Destructor.
    /** Closes Window. */
    ~Renderer();
    Renderer( Renderer& ) = delete;
    Renderer& operator=( Renderer& ) = delete;

    /// Pre-defined Viewports.
    /** Viewport is a part of sf::Window, where stuff can be drawn.
     *  Usefull in split-screen applications. */
    enum Viewports
    {
        /// Viewport where its sf::Rect is equal to screen size.
        FullScreenViewport
    };

    /// Draw registered objects.
    /** Iterates through Layer s, and draw registered Object s by
     *  addObject(*) methods. */
    void draw() const;

    /// Register object to be drawn.
    /** @ingroup Renderer */
    void addObject( World::Entity::Object* const object );

    /// Register object to be drawn.
    void addObject( World::Entity::Object* const object,
                    LayerID layerID );

    void removeObject( const World::Entity::Object* const object );
    void removeObject( std::string name, LayerID layerID );

    void removeObjects( LayerID layerID );

    inline auto getWindow() const -> const sf::Window&;
    inline auto getLayer( LayerID ) -> Layer&;
    inline auto getLayer( LayerID ) const -> const Layer&;

    inline auto getCameraPosition() const -> const sf::Vector2f&;
    inline auto getCameraSize() const -> const sf::Vector2f&;

    inline void setCameraPosition( sf::Vector2f );
    inline void setCameraSize( sf::Vector2f );

  private:
    sf::View mCamera;
    const std::map< Viewports, sf::Rect< float > > mViewports;
    mutable sf::RenderWindow mWindow;
    mutable std::map< LayerID, Layer > mLayers;
};

#include "Renderer/renderer.inl"

}}

#endif // !POPHEAD_RENDERER_RENDERER_H_
