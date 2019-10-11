#include "GameObjects/DrawableGameObjects/Items/bulletItem.hpp"

namespace ph {

BulletItem::BulletItem(GameData* const gameData)
	:Item(gameData, "Bullet", gameData->getTextures().get("textures/others/bulletGround.png"))
{
}

void BulletItem::drawWhileOnTheGround(sf::Transform transform)
{
	Renderer::submit(getGroundSprite(), transform);
}

}