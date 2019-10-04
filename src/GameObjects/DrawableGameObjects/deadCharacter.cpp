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

	sf::Color newColor = mSprite.getColor();
	newColor.a = 255 - (mTimeFromDeath.asSeconds() * 25.5);
	mSprite.setColor(newColor);

	if(mTimeFromDeath.asSeconds() > 10) {
		auto deadCharacters = dynamic_cast<LyingGameObjectsLayer*>(mParent);
		deadCharacters->removeDeadEnemy(this);
	}
}

void DeadCharacter::drawCurrent(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

}
