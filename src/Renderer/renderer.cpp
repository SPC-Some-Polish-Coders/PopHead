#include "Renderer/renderer.hpp"

#include "World/Entity/object.hpp"

using PopHead::Renderer::Renderer;
using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;

Renderer::Renderer()
        : mCamera { { 0, 0, 32*10, 32*10 } }
        , mViewports { { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
        , mWindow { sf::VideoMode::getDesktopMode(),
                    "PopHead",
                    sf::Style::Fullscreen }
        , mLayers { { LayerID::floorEntities, Layer() },
                    { LayerID::staticEntities, Layer() },
                    { LayerID::kinematicEntities, Layer() },
                    { LayerID::airEntities, Layer() },
                    { LayerID::GUI, Layer() }, }
{
    mCamera.setViewport( mViewports.at( FullScreenViewport ) );
}

Renderer::~Renderer()
{
    mWindow.close();
}

void Renderer::draw() const
{
    mWindow.clear();

    for( const auto& layer : mLayers )
        for( const auto& object : layer.second )
            mWindow.draw(*object);

    mWindow.display();
}

void Renderer::addObject( PopHead::World::Entity::Object* const object )
{
    mLayers[object->getLayerID()].addObject( object );
}

void Renderer::addObject(PopHead::World::Entity::Object* const object,
                          LayerID layerID )
{
    mLayers[layerID].addObject( object );
}

void Renderer::removeObject( std::string name, LayerID layerID )
{
    mLayers[layerID].removeObject( name );
}

void Renderer::removeObject( const PopHead::World::Entity::Object* const object )
{
    mLayers[object->getLayerID()].removeObject( object );
}

void Renderer::removeObjects( LayerID layerID )
{
    mLayers[layerID].clear();
}

auto Renderer::getWindow() const -> const sf::Window& { return mWindow; }

auto Renderer::getLayer( LayerID id ) -> Layer& { return mLayers[id]; }

auto Renderer::getLayer( LayerID id ) const -> const Layer& { return mLayers[id]; }

auto Renderer::getCameraPosition() const -> const sf::Vector2f& { return mCamera.getCenter(); }

auto Renderer::getCameraSize() const -> const sf::Vector2f& { return mCamera.getSize(); }

void Renderer::setCameraPosition( sf::Vector2f center ) { mCamera.setCenter( center ); }

void Renderer::setCameraSize( sf::Vector2f size ) { mCamera.setSize( size ); }
