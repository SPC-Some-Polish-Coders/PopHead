#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"

namespace ph {

BulletItem::BulletItem(GameData* const gameData)
	:Item(gameData, "Bullet")
{
	setGroundTexture(gameData->getTextures().get("textures/others/bulletGround.png"));
}

void BulletItem::drawWhileOnTheGround(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(getGroundSprite());
}

void BulletItem::onDrop()
{

}

}