#include "GameObjects/NonDrawableGameObjects/activateArea.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Utilities/rect.hpp"
#include "Logs/logs.hpp"

namespace ph {

ActivateArea::ActivateArea(const std::string& areaName, const sf::FloatRect area)
	:GameObject("activateArea_" + areaName)
	,mArea(area)
	,mActivated(false)
{
	mPlayer = dynamic_cast<Player*>(mRoot->getChild("LAYER_standingObjects")->getChild("player"));
}

void ActivateArea::updateCurrent(const sf::Time delta)
{
	if (mPlayer == nullptr || mPlayer->isDead())
		return;
	if (FloatRect::positiveRectsIntersects(mPlayer->getGlobalBounds(), mArea))
		mActivated = true;
	else
		mActivated = false;
}

}