#include "dyingCharacter.hpp"
#include "GameObjects/GameObjectContainers/enemyContainer.hpp"

namespace ph {

DyingCharacter::DyingCharacter(const sf::Sprite& sprite)
	:GameObject("dyingCharacter")
	,mSprite(sprite)
{
}

void DyingCharacter::updateCurrent(const sf::Time delta)
{
	mTimeFromDeath += delta;
	if(mTimeFromDeath.asSeconds() > 10) {
		auto deadCharacters = dynamic_cast<DeadEnemyContainer*>(mParent);
		deadCharacters->removeDeadEnemy(this);
	}
}

void DyingCharacter::drawCurrent(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

}
