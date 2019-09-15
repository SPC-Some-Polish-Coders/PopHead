#include "GameObjects/NonDrawableGameObjects/activateGateAreas.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

namespace ph {

OpenGateArea::OpenGateArea(const sf::FloatRect area)
	:GameObject("OpenGateArea")
	,mArea(area)
	,mActivated(false)
{
}

void OpenGateArea::updateCurrent(const sf::Time delta)
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto* playerGameObject = standingObjects->getChild("player");
	if (playerGameObject == nullptr)
		return;
	if (Math::areTheyOverlapping(playerGameObject->getGlobalBounds(), mArea))
		mActivated = true;
	else
		mActivated = false;
}

CloseGateArea::CloseGateArea(const sf::FloatRect area)
	:GameObject("OpenGateArea")
	, mArea(area)
	, mActivated(false)
{
}

void CloseGateArea::updateCurrent(const sf::Time delta)
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto* playerGameObject = standingObjects->getChild("player");
	if (playerGameObject == nullptr)
		return;
	if (Math::areTheyOverlapping(playerGameObject->getGlobalBounds(), mArea))
		mActivated = true;
	else
		mActivated = false;
}

}