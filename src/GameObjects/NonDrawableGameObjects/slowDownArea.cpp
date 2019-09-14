#include "slowDownArea.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "Utilities/math.hpp"

namespace ph {

SlowDownArea::SlowDownArea(const sf::FloatRect& area)
	:GameObject("slowDownArea")
	,mArea(area)
{
}

void SlowDownArea::updateCurrent(const sf::Time delta)
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	
	auto* playerGameObject = standingObjects->getChild("player");
	if(playerGameObject != nullptr) {
		auto* player = dynamic_cast<Player*>(playerGameObject);
		if(Math::areTheyOverlapping(player->getGlobalBounds(), mArea))
			player->slowDown();
	}

	for(auto& child : standingObjects->getChildren()) {
		auto* zombie = dynamic_cast<Zombie*>(child.get());
		if(zombie == nullptr)
			continue;
		if(Math::areTheyOverlapping(zombie->getGlobalBounds(), mArea))
			zombie->slowDown();
	}
}

}
