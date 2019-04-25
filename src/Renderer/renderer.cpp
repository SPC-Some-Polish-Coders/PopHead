#include "Renderer/renderer.hpp"

#include "World/Entity/object.hpp"

using PopHead::Renderer::Renderer;
using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;

Renderer::Renderer()
	:	mCamera{ sf::Vector2f{0,0}, sf::Vector2f{32*30, 32*30} }
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
    mWindow.setVerticalSyncEnabled(false);
}

Renderer::~Renderer()
{
    mWindow.close();
}

void Renderer::update(sf::Time delta)
{
	mCamera.update(delta);
}

void Renderer::draw() const
{
	mCamera.applyTo(mWindow);
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


