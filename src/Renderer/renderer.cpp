#include "Renderer/renderer.hpp"
#include "Utilities/iniLoader.hpp"
#include "windowInitializer.hpp"
#include "EntityComponentSystem/object.hpp"
#include "Utilities/debug.hpp"
#include "gameData.hpp"

namespace ph {

Renderer::Renderer()
	:mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mViewports{ { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
	,mWindow{ WindowInitializer::getWindowSize(),
		"PopHead", WindowInitializer::getStyle(), sf::ContextSettings() }
	,mLayers{  { LayerID::floorEntities, Layer() },
				{ LayerID::staticEntities, Layer() },
				{ LayerID::kinematicEntities, Layer() },
				{ LayerID::airEntities, Layer() },
				{ LayerID::gui, Layer() },
				{ LayerID::cmd, Layer() } }
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

	mGameData->getMap().draw(mWindow, sf::RenderStates::Default, mCamera.getCenter(), mCamera.getSize());

	for(const auto& layer : mLayers)
		for(const auto& object : layer.second) {
			mWindow.draw(*object);
			mGameData->getEfficiencyRegister().registerRenderCall();
		}

	mWindow.draw(mGameData->getPhysicsEngine().getCollisionDebugManager());
	mWindow.draw(mGameData->getEfficiencyRegister().getDisplayer());
	mWindow.draw(mGameData->getTerminal().getImage());

	mWindow.display();
}

void Renderer::addObject(Object* const object)
{
	mLayers[object->getLayerID()].addObject(object);
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was added to " + getLayerName(object->getLayerID()) + " layer.");
}

void Renderer::addObject(Object* const object, LayerID layerID)
{
	mLayers[layerID].addObject(object);
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was added to " + getLayerName(layerID) + " layer.");
}

void Renderer::removeObject(std::string name, LayerID layerId)
{
	mLayers[layerId].removeObject(name);
	PH_LOG(LogType::Info, "Object \"" + name + "\" was removed from " + getLayerName(layerId) + " layer.");
}

void Renderer::removeObject(const Object* const object)
{
	PH_LOG(LogType::Info, "Object \"" + object->getName() + "\" was removed from " + getLayerName(object->getLayerID()) + " layer.");
	mLayers[object->getLayerID()].removeObject(object);
}

void Renderer::removeAllObjectsFromLayer(LayerID layerID)
{
	mLayers[layerID].clear();
	PH_LOG(LogType::Info, "All objects were removed from " + getLayerName(layerID) + " layer.");
}

void Renderer::clear() noexcept
{
	for(auto& layer : mLayers)
		layer.second.clear();
}

void Renderer::setPositionOfStaticObjectsToCamera()
{
	const sf::Vector2f movementFromLastFrame = mCamera.getCameraMoveFromLastFrame();
	for(const auto& guiObject : mLayers[LayerID::gui]) {
		guiObject->move(movementFromLastFrame);
	}
	mGameData->getTerminal().getImage().move(movementFromLastFrame);
	mGameData->getEfficiencyRegister().getDisplayer().move(movementFromLastFrame);
}

std::string Renderer::getLayerName(LayerID layerID) const
{
	switch(layerID)
	{
	case LayerID::floorEntities:     return "floorEntities";
	case LayerID::staticEntities:    return "staticEntities";
	case LayerID::kinematicEntities: return "kinematicEntities";
	case LayerID::airEntities:       return "airEntities";
	case LayerID::collisionDebug:    return "collisionDebug";
	case LayerID::gui:               return "gui";
	case LayerID::cmd:               return "cmd";
	default:                         return "ERROR: Every object has to be bind to the certain layer.";
	}
}

}