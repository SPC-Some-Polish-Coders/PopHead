#include "Renderer/Renderer.hpp"

#include "World/Entity/Object.hpp"

using PopHead::Renderer::Renderer;
using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;

Renderer::Renderer()
        : mCamera { { 0, 0, 100, 100 } }
        // std::vector< sf::ViewPort > mViewports;
        , mWindow { sf::VideoMode::getDesktopMode(), "test" }
        , mLayers { { LayerID::floorEntities, Layer() },
                    { LayerID::staticEntities, Layer() },
                    { LayerID::kinematicEntities, Layer() },
                    { LayerID::airEntities, Layer() },
                    { LayerID::GUI, Layer() }, }
{
}

void Renderer::draw() const
{
    for( const auto& layer : mLayers )
        for( const auto& object : layer.second )
            object->draw();
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
