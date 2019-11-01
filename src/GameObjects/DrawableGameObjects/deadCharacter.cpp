#include "deadCharacter.hpp"
#include "GameObjects/GameObjectContainers/gameObjectLayers.hpp"
#include "Resources/spriteSheetData.hpp"

namespace ph {

DeadCharacter::DeadCharacter(const Texture& texture, const sf::IntRect& textureRect, const std::string name)
	:GameObject("dyingCharacter")
	,mTextureRect(textureRect)
	,mTexture(texture)
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

void DeadCharacter::drawCurrent(sf::Transform transform)
{
	int alpha = 255 - (mTimeFromDeath.asSeconds() * 25.5);
	sf::Color color(255, 255, 255, (unsigned)alpha);
	Renderer::submitQuad(mTexture, color, mTextureRect, getPosition(), {SpriteSheetData::HUMAN_WIDTH, SpriteSheetData::HUMAN_HEIGHT});
}

}
