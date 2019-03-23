#include "Renderer/Renderer.hpp"

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
            object->draw();

    mWindow.display();
}

void Renderer::addObject( World::Entity::Object* const object )
{
    mLayers[object->getLayerID()].addObject( object );
}

void Renderer::addObject( World::Entity::Object* const object,
                          LayerID layerID )
{
    mLayers[layerID].addObject( object );
}

void Renderer::removeObject( std::string name, LayerID layerID )
{
    mLayers[layerID].removeObject( name );
}

void Renderer::removeObject( const World::Entity::Object* const object )
{
    mLayers[object->getLayerID()].removeObject( object );
}

void Renderer::removeObjects( LayerID layerID )
{
    mLayers[layerID].clear();
}
