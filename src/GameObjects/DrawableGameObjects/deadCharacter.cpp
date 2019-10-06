#include "deadCharacter.hpp"
#include "GameObjects/GameObjectContainers/gameObjectLayers.hpp"

namespace ph {

DeadCharacter::DeadCharacter(const sf::Sprite& sprite)
	:GameObject("dyingCharacter")
	,mSprite(sprite)
{
}

void DeadCharacter::updateCurrent(const sf::Time delta)
{
	mTimeFromDeath += delta;
	if(mTimeFromDeath.asSeconds() > 10) {
		auto deadCharacters = dynamic_cast<LyingGameObjectsLayer*>(mParent);
		deadCharacters->removeDeadEnemy(this);
	}
}

void DeadCharacter::drawCurrent(sf::Transform)
{
	//target.draw(mSprite, states);
}

}
