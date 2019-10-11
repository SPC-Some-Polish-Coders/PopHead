#include "deadCharacter.hpp"
#include "GameObjects/GameObjectContainers/gameObjectLayers.hpp"

namespace ph {

DeadCharacter::DeadCharacter(const Texture& texture, const sf::IntRect& textureRect, const std::string name)
	:GameObject("dyingCharacter")
	,mSprite(texture, name)
{
	setTextureRect(mSprite.mVertexArray.getVertexBuffer(), textureRect, texture.getSize());
}

void DeadCharacter::updateCurrent(const sf::Time delta)
{
	mTimeFromDeath += delta;
	if(mTimeFromDeath.asSeconds() > 10) {
		auto deadCharacters = dynamic_cast<LyingGameObjectsLayer*>(mParent);
		deadCharacters->removeDeadEnemy(this);
	}
}

void DeadCharacter::drawCurrent(sf::Transform transform)
{
	Renderer::submit(mSprite, transform);
}

}
