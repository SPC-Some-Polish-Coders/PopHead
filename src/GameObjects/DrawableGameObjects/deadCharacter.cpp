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

	//TODO: Make character fade out using new renderer
	/*sf::Color newColor = mSprite.getColor();
	newColor.a = 255 - (mTimeFromDeath.asSeconds() * 25.5);
	mSprite.setColor(newColor);*/

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
