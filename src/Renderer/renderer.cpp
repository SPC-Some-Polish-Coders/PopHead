#include "Renderer/renderer.hpp"
#include "Utilities/iniLoader.hpp"
#include "windowInitializer.hpp"

#include "World/Entity/object.hpp"
#include "Logs/logger.hpp"

using PopHead::Renderer::Renderer;
using PopHead::Renderer::Layer;
using PopHead::Renderer::LayerID;
using PopHead::Renderer::WindowInitializer;
using PopHead::Utilities::IniLoader;

Renderer::Renderer()
	:	mCamera{ sf::Vector2f{0,0}, sf::Vector2f{32*30, 32*30} }
        , mViewports { { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
		, mWindow{ WindowInitializer::getWindowSize(),
			"PopHead", WindowInitializer::getStyle() }
        , mLayers { { LayerID::floorEntities, Layer() },
                    { LayerID::staticEntities, Layer() },
                    { LayerID::kinematicEntities, Layer() },
                    { LayerID::airEntities, Layer() },
                    { LayerID::GUI, Layer() }, }
{
    mCamera.setViewport(mViewports.at(FullScreenViewport));
    mWindow.setVerticalSyncEnabled(false);
}

Renderer::~Renderer()
{
    mWindow.close();
}

void Renderer::update(sf::Time delta)
{
	mCamera.update(delta);
	setPositionOfStaticObjectsToCamera();
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
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was added to " + getLayerName(object->getLayerID()) + " layer.");
}

void Renderer::addObject(PopHead::World::Entity::Object* const object, LayerID layerID )
{
    mLayers[layerID].addObject( object );
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was added to " + getLayerName(layerID) + " layer.");
}

void Renderer::removeObject( std::string name, LayerID layerID )
{
    mLayers[layerID].removeObject( name );
	PH_LOG(LogType::Info, "Object \"" + name + "\" was removed from " + getLayerName(layerID) + " layer.");
}

void Renderer::removeObject( const PopHead::World::Entity::Object* const object )
{
    mLayers[object->getLayerID()].removeObject( object );
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was removed from " + getLayerName(object->getLayerID()) + " layer.");
}

void Renderer::removeAllObjectsFromLayer( LayerID layerID )
{
    mLayers[layerID].clear();
	PH_LOG(LogType::Info, "All objects were removed from " + getLayerName(layerID) + " layer.");
}

void Renderer::setPositionOfStaticObjectsToCamera()
{
	for (const auto& guiObject : mLayers[LayerID::GUI]) {
		guiObject->move(mCamera.getCameraMoveFromLastFrame());
	}
}

std::string Renderer::getLayerName(LayerID layerID) const
{
	switch (layerID)
	{
	case PopHead::Renderer::LayerID::floorEntities:
		return "floorEntities";
	case PopHead::Renderer::LayerID::staticEntities:
		return "staticEntities";
	case PopHead::Renderer::LayerID::kinematicEntities:
		return "kinematicEntities";
	case PopHead::Renderer::LayerID::airEntities:
		return "airEntities";
	case PopHead::Renderer::LayerID::collisionDebug:
		return "collisionDebug";
	case PopHead::Renderer::LayerID::GUI:
		return "GUI";
	default:
		return "ERROR: Every object has to be bind to the certain layer.";
	}
}


