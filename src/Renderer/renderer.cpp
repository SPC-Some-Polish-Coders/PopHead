#include "Renderer/renderer.hpp"
#include "Utilities/iniLoader.hpp"
#include "windowInitializer.hpp"
#include "EntityComponentSystem/object.hpp"
#include "Utilities/debug.hpp"
#include "gameData.hpp"

namespace ph {

Renderer::Renderer(sf::RenderTarget& renderTarget)
	:mRenderTarget(renderTarget)
	,mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mViewports{ { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
	,mLayers{  { LayerID::floorEntities, Layer() },
				{ LayerID::staticEntities, Layer() },
				{ LayerID::kinematicEntities, Layer() },
				{ LayerID::airEntities, Layer() },
				{ LayerID::gui, Layer() },
				{ LayerID::cmd, Layer() } }
{
	mCamera.setViewport(mViewports.at(FullScreenViewport));
}

Renderer::~Renderer()
{
}

void Renderer::update(sf::Time delta)
{
	mCamera.update(delta);
	setPositionOfStaticObjectsToCamera();
}

void Renderer::draw() const
{
	mCamera.applyTo(mRenderTarget);
	mRenderTarget.clear();

	mGameData->getMap().draw(mRenderTarget, sf::RenderStates::Default, mCamera.getCenter(), mCamera.getSize());

	for(const auto& layer : mLayers)
		for(const auto& object : layer.second) {
			mRenderTarget.draw(*object);
			mGameData->getEfficiencyRegister().registerRenderCall();
		}

	mRenderTarget.draw(mGameData->getPhysicsEngine().getCollisionDebugManager());
	mRenderTarget.draw(mGameData->getEfficiencyRegister().getDisplayer());
	mRenderTarget.draw(mGameData->getTerminal().getImage());
}

void Renderer::addObject(Object* const object)
{
	mLayers[object->getLayerID()].addObject(object);
	PH_LOG_INFO("Object \"" + object->getName() + "\" was added to " + getLayerName(object->getLayerID()) + " layer.");
}

void Renderer::addObject(Object* const object, LayerID layerID)
{
	mLayers[layerID].addObject(object);
	PH_LOG_INFO("Object \"" + object->getName() + "\" was added to " + getLayerName(layerID) + " layer.");
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